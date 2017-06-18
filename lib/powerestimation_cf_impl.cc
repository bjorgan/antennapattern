/* -*- c++ -*- */
/*
 * Copyright 2017 <+YOU OR YOUR COMPANY+>.
 *
 * This is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3, or (at your option)
 * any later version.
 *
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this software; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street,
 * Boston, MA 02110-1301, USA.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <gnuradio/io_signature.h>
#include "powerestimation_cf_impl.h"

namespace gr {
  namespace antennapattern {

    powerestimation_cf::sptr
    powerestimation_cf::make()
    {
      return gnuradio::get_initial_sptr
        (new powerestimation_cf_impl());
    }

    powerestimation_cf_impl::powerestimation_cf_impl()
      : gr::block("powerestimation_cf",
              gr::io_signature::make(1, 1, sizeof(gr_complex)),
              gr::io_signature::make(1, 1, sizeof(float)))
    {
      d_first_iteration = true;
      cluster_initialize(d_low_level_cluster);
      cluster_initialize(d_high_level_cluster);
      d_ready = false;
    }

    void cluster_initialize(struct cluster &cluster)
    {
      cluster.mean = -1;
      cluster.M2 = 0;
      cluster.n = 1;
      cluster.iterations = 0;

      cluster.mean_at_previous = 0;
      cluster.stdev_at_previous = 0;
      cluster.samples_since_previous = 0;
    }

    void cluster_update(struct cluster &cluster, double sample)
    {
      cluster.n++;
      cluster.iterations++;
      double delta = sample - cluster.mean;
      cluster.mean += 1.0/cluster.n*delta;
      double delta2 = sample - cluster.mean;
      cluster.M2 += delta*delta2;

      if (cluster.samples_since_previous > PREVIOUS_TIMEPOINT_DIFF) {
        cluster.mean_at_previous = cluster.mean;
        cluster.stdev_at_previous = cluster_stdev(cluster);
        cluster.samples_since_previous = 0;
      } else {
        cluster.samples_since_previous++;
      }

      //if difference in sample means differ by more than the previous standard deviation, we are not converged yet. Restart variables to improve convergence rate and make statistics more correct for later T test
      if ((fabs(cluster.mean - cluster.mean_at_previous) > cluster.stdev_at_previous) && (cluster.n > 10) && (cluster.iterations > PREVIOUS_TIMEPOINT_DIFF)) {
        cluster.n = 1;
        cluster.samples_since_previous = 2*PREVIOUS_TIMEPOINT_DIFF;
      }
    }

    double cluster_distance(const struct cluster &cluster, double sample)
    {
      return fabs(cluster.mean - sample);
    }

    double cluster_var(const struct cluster &cluster)
    {
      if (cluster.n > 1) {
        return cluster.M2/(cluster.n-1);
      } else {
        std::nan("");
      }
    }

    double cluster_stdev(const struct cluster &cluster)
    {
      return sqrt(cluster_var(cluster));
    }

    double clusters_t_statistics(const struct cluster &d_cluster_1, const struct cluster &d_cluster_2)
    {
      double T = d_cluster_1.mean - d_cluster_2.mean;
      T /= sqrt(cluster_var(d_cluster_1)/d_cluster_1.n + cluster_var(d_cluster_2)/d_cluster_2.n);
      double var_1 = cluster_var(d_cluster_1);
      double var_2 = cluster_var(d_cluster_2);

      //avoid weird T values in output by ensuring large enough variance and large enough samples in each cluster
      const int MIN_N_THRESHOLD = 5;
      if ((d_cluster_1.n > MIN_N_THRESHOLD) && (d_cluster_2.n > MIN_N_THRESHOLD) && (var_1 > 0) && (var_2 > 0)) {
        return T;
      }
      return 0;
    }

    powerestimation_cf_impl::~powerestimation_cf_impl()
    {
    }

    void
    powerestimation_cf_impl::forecast (int noutput_items, gr_vector_int &ninput_items_required)
    {
      ninput_items_required[0] = noutput_items;
    }

    int
    powerestimation_cf_impl::general_work (int noutput_items,
                       gr_vector_int &ninput_items,
                       gr_vector_const_void_star &input_items,
                       gr_vector_void_star &output_items)
    {
      const gr_complex *in = (const gr_complex *) input_items[0];
      float *out = (float *) output_items[0];

      float output_value = 0;

      int num_input_items = ninput_items[0];
      if (d_first_iteration && (d_low_level_cluster.mean < 0)) {
        d_low_level_cluster.mean = std::abs(in[0]);
      }

      int iterations = 0; //control which input samples are used
      int output_index = 0; //index in output array
      for (int i=0; i < num_input_items; i++) {
        float magnitude = std::abs(in[i]);
        iterations++;
        if (d_first_iteration && (magnitude != d_low_level_cluster.mean)) {
          //initialize second cluster to second, differing sample
          d_high_level_cluster.mean = magnitude;
          d_first_iteration = false;
        } else if (d_first_iteration) {
          continue;
        }

        //find which mean the current sample is the closest, and update either high or low correspondingly
        double distance_1 = cluster_distance(d_low_level_cluster, magnitude);
        double distance_2 = cluster_distance(d_high_level_cluster, magnitude);
        double max_stdev = std::max(cluster_stdev(d_low_level_cluster), cluster_stdev(d_high_level_cluster));
        double min_stdev = std::min(cluster_stdev(d_low_level_cluster), cluster_stdev(d_high_level_cluster));
        const int NUM_STDEVS = 3;
        if ((distance_1 < NUM_STDEVS*min_stdev) && (distance_2 < NUM_STDEVS*min_stdev)) {
          //select one of the clusters randomly if the sample is within the fuzzy boundary of both clusters
          if (rand()*1.0/RAND_MAX > 0.5) {
            cluster_update(d_low_level_cluster, magnitude);
          } else {
            cluster_update(d_high_level_cluster, magnitude);
          }
        } else if (distance_1 < distance_2) {
          cluster_update(d_low_level_cluster, magnitude);
        } else if (distance_1 > distance_2) {
          cluster_update(d_high_level_cluster, magnitude);
        } else {
          //update both if distances are equal
          cluster_update(d_low_level_cluster, magnitude);
          cluster_update(d_high_level_cluster, magnitude);
        }

        //if means are within the standard error of each other and a sample is drastically different, we should try to reinitialize one of the clusters in order to try to push the current status quo
        if ((cluster_distance(d_low_level_cluster, d_high_level_cluster.mean) < NUM_STDEVS*min_stdev) && (std::min(distance_1, distance_2) > NUM_STDEVS*min_stdev)) {
          d_high_level_cluster.n = 1;
          d_high_level_cluster.mean = magnitude;
        }

        //if current T statistics is larger than the pre-defined threshold,
        //we might have characterized the low and high cluster and block is ready for
        //output.
        d_T_statistics = clusters_t_statistics(d_low_level_cluster, d_high_level_cluster);
        const double MIN_T_THRESHOLD = 10;
        if (fabs(d_T_statistics) > MIN_T_THRESHOLD) {
          d_ready = true;
        }

        //switch around which cluster characterize the low and high level
        if (d_high_level_cluster.mean < d_low_level_cluster.mean) {
          struct cluster temp_cluster;
          temp_cluster = d_high_level_cluster;
          d_high_level_cluster = d_low_level_cluster;
          d_low_level_cluster = temp_cluster;
        }

        //set output to difference between low and high level if current sample belongs to the high level
        if (d_ready && (cluster_distance(d_high_level_cluster, magnitude) < cluster_distance(d_low_level_cluster, magnitude))) {
          out[output_index++] = 20*log10(magnitude) - 20*log10(d_low_level_cluster.mean);
        }
      }

      consume_each (iterations);

      if (d_ready) {
        return output_index;
      }
      return 0;
    }

  } /* namespace antennapattern */
} /* namespace gr */


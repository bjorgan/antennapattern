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

#ifndef INCLUDED_ANTENNAPATTERN_POWERESTIMATION_CF_IMPL_H
#define INCLUDED_ANTENNAPATTERN_POWERESTIMATION_CF_IMPL_H

#include <antennapattern/powerestimation_cf.h>

namespace gr {
  namespace antennapattern {
    /** Used for tracking mean and standard deviation
     * of a cluster.
     **/
    struct cluster {
      ///current mean
      double mean;
      ///current variance multiplied by current n - 1
      double M2;
      ///number of samples in mean
      long n;
      ///number of iterations of cluster_update
      long iterations;
      ///mean at a previous point in time, used for determining whether
      ///the mean is unstable/samples are changing
      double mean_at_previous;
      ///standard deviation at a previous point in time
      double stdev_at_previous;
      ///number of samples since the previous timepoint defined above
      int samples_since_previous;
    };

    ///number of samples between each time we update mean_at_previous in cluster.
    const int PREVIOUS_TIMEPOINT_DIFF = 30;

    /**
     * Initialize cluster.
     **/
    void cluster_initialize(struct cluster &cluster);

    /**
     * Update cluster with new sample.
     **/
    void cluster_update(struct cluster &cluster, double sample);

    /**
     * Calculate distance between cluster and a new sample.
     **/
    double cluster_distance(const struct cluster &cluster, double sample);

    /**
     * Calculate standard deviation of cluster.
     **/
    double cluster_stdev(const struct cluster &cluster);

    /**
     * Calculate student-T statistics for two clusters.
     **/
    double clusters_t_statistics(const struct cluster &cluster_1, const struct cluster &cluster_2);

    /**
     * Calculate the probability that the cluster would produce
     * a sample larger than the input sample.
     **/
    double cluster_probability(const struct cluster &cluster, double sample);

    class powerestimation_cf_impl : public powerestimation_cf
    {
     private:
      //clusters that are used to track the sample means of low
      //and high level of the signal in the long run. Used
      //for finding whether current sample belongs to
      //high or low level.
      struct cluster d_high_level_cluster;
      struct cluster d_low_level_cluster;

      //set to true when two statistically different clusters (high and low) have been
      //found and satisfies the T test
      bool d_ready;

      //whether we are at the first iteration of the block and need to assign starting values of low and high
      bool d_first_iteration;

      //currently calculated T statistics of high and low
      double d_T_statistics;

     public:
      powerestimation_cf_impl();
      ~powerestimation_cf_impl();

      void forecast (int noutput_items, gr_vector_int &ninput_items_required);

      int general_work(int noutput_items,
           gr_vector_int &ninput_items,
           gr_vector_const_void_star &input_items,
           gr_vector_void_star &output_items);
    };

  } // namespace antennapattern
} // namespace gr

#endif /* INCLUDED_ANTENNAPATTERN_POWERESTIMATION_CF_IMPL_H */


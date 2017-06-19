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
#include "pattern_sink_f_impl.h"
#include "antenna_angle_tagger_impl.h"

namespace gr {
  namespace antennapattern {

    void angle_mapper::add_samples(std::vector<gr::tag_t> tags, long tag_offset, int noutput_items, const float *in)
    {
      int curr_tag = 0;

      for (int i=0; i < noutput_items; i++) {
        //get azimuth corresponding to current input sample from tags
        if (curr_tag < tags.size()) {
          if (tags[curr_tag].offset >= i + tag_offset) {
            d_azimuth = pmt::to_float(tags[curr_tag].value);
            curr_tag++;
          }
        }

        //update values in pattern at current azimuth angle
        int curr_azimuth = d_azimuth;
        if (d_pattern[curr_azimuth].n == 0) {
          cluster_initialize(d_pattern[curr_azimuth]);
        }
        cluster_update(d_pattern[curr_azimuth], in[i]);
      }
    }

    void angle_mapper::to_file(std::string filename)
    {
      //write statistics at each azimuth angle to file
      FILE *fp = fopen(filename.c_str(), "w");
      for (auto const &item : d_pattern) {
        int azimuth = item.first;
        struct cluster cluster = item.second;
        fprintf(fp, "%d %ld %f %f\n", azimuth, cluster.n, cluster.mean, cluster_stdev(cluster));
      }
      fclose(fp);
    }
    
    void angle_mapper::get_pattern(std::vector<double> &angles, std::vector<double> &magnitudes)
    {
      angles.clear();
      magnitudes.clear();
      for (auto const &item : d_pattern) {
        int azimuth = item.first;
        struct cluster cluster = item.second;
        angles.push_back(azimuth);
        magnitudes.push_back(cluster.mean);
      }
    }

    pattern_sink_f::sptr
    pattern_sink_f::make(std::string filename)
    {
      return gnuradio::get_initial_sptr
        (new pattern_sink_f_impl(filename));
    }

    pattern_sink_f_impl::pattern_sink_f_impl(std::string filename)
      : gr::sync_block("pattern_sink_f",
              gr::io_signature::make(1, 1, sizeof(float)),
              gr::io_signature::make(0, 0, 0)), d_output_filename(filename)
    {}

    pattern_sink_f_impl::~pattern_sink_f_impl()
    {
    }

    bool pattern_sink_f_impl::stop()
    {
      d_mapped_values.to_file(d_output_filename);
      gr::sync_block::stop();
    }

    int
    pattern_sink_f_impl::work(int noutput_items,
        gr_vector_const_void_star &input_items,
        gr_vector_void_star &output_items)
    {
      const float *in = (const float *) input_items[0];

      //obtain the azimuth tags during the time window
      std::vector<gr::tag_t> tags;
      get_tags_in_range(tags, 0, nitems_read(0), nitems_read(0) + noutput_items, pmt::mp(ANTENNA_ANGLE_TAG));

      d_mapped_values.add_samples(tags, nitems_read(0), noutput_items, in);

      return noutput_items;
    }

  } /* namespace antennapattern */
} /* namespace gr */


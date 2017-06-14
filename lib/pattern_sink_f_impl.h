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

#ifndef INCLUDED_ANTENNAPATTERN_PATTERN_SINK_F_IMPL_H
#define INCLUDED_ANTENNAPATTERN_PATTERN_SINK_F_IMPL_H

#include <antennapattern/pattern_sink_f.h>
#include "powerestimation_cf_impl.h"

namespace gr {
  namespace antennapattern {

    class pattern_sink_f_impl : public pattern_sink_f
    {
     private:
      ///current antenna pattern: Maps angles at 1 degrees precision
      ///to means and standard deviations.
      std::map<int, gr::antennapattern::cluster> d_pattern;
      ///current azimuth
      double d_azimuth;
      ///output filename to which output is written when this block is stopped
      std::string d_output_filename;

     public:
      pattern_sink_f_impl(std::string filename);
      ~pattern_sink_f_impl();

      /**
       * Reimplemented from gr::sync_block::stop(): Writes
       * d_pattern to file d_output_filename before calling gr::sync_block::stop().
       **/
      bool stop();

      int work(int noutput_items,
         gr_vector_const_void_star &input_items,
         gr_vector_void_star &output_items);
    };

  } // namespace antennapattern
} // namespace gr

#endif /* INCLUDED_ANTENNAPATTERN_PATTERN_SINK_F_IMPL_H */


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

#ifndef INCLUDED_ANTENNAPATTERN_ANTENNA_ANGLE_TAGGER_IMPL_H
#define INCLUDED_ANTENNAPATTERN_ANTENNA_ANGLE_TAGGER_IMPL_H

#include <antennapattern/antenna_angle_tagger.h>

namespace gr {
  namespace antennapattern {
    const std::string ANTENNA_ANGLE_TAG = "rotator_angle";

    class antenna_angle_tagger_impl : public antenna_angle_tagger
    {
     private:
      ///current azimuth angle
      float d_azimuth;
      ///current elevation angle
      float d_elevation;
      ///whether we are connected to rotctld
      bool d_rotctld_connected;
      ///rotctld socket
      int d_rotctld_socket;

      /**
       * Connect to rotctld on the default host and port (4533).
       *
       * This function is obtained from flyby (https://github.com/la1k/flyby).
       **/
      void rotctld_connect();

      /**
       * Read current elevation and azimuth angle from rotctld.
       * Output is saved to d_elevation and d_azimuth.
       **/
      void rotctld_read();

     public:
      antenna_angle_tagger_impl();
      ~antenna_angle_tagger_impl();

      int work(int noutput_items, gr_vector_const_void_star &input_items, gr_vector_void_star &output_items);
    };

  } // namespace antennapattern
} // namespace gr

#endif /* INCLUDED_ANTENNAPATTERN_ANTENNA_ANGLE_TAGGER_IMPL_H */


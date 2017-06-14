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
#include "antenna_angle_tagger_impl.h"

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <math.h>

namespace gr {
  namespace antennapattern {

    antenna_angle_tagger::sptr
    antenna_angle_tagger::make()
    {
      return gnuradio::get_initial_sptr
        (new antenna_angle_tagger_impl());
    }

    antenna_angle_tagger_impl::antenna_angle_tagger_impl()
      : gr::sync_block("antenna_angle_tagger",
              gr::io_signature::make(1, 1, sizeof(float)),
              gr::io_signature::make(1, 1, sizeof(float)))
    {
        rotctld_connect();
        d_azimuth = 0;
        d_elevation = 0;
    }

    antenna_angle_tagger_impl::~antenna_angle_tagger_impl()
    {
    }

    void antenna_angle_tagger_impl::rotctld_connect()
    {
      d_rotctld_connected = false;
      struct addrinfo hints, *servinfo, *servinfop;
      memset(&hints, 0, sizeof(hints));
      hints.ai_family = AF_UNSPEC;
      hints.ai_socktype = SOCK_STREAM;

      d_rotctld_socket = 0;
      int retval = getaddrinfo("localhost", "4533", &hints, &servinfo);
      if (retval != 0) {
        return;
      }

      for(servinfop = servinfo; servinfop != NULL; servinfop = servinfop->ai_next) {
        if ((d_rotctld_socket = socket(servinfop->ai_family, servinfop->ai_socktype,
	          servinfop->ai_protocol)) == -1) {
	        continue;
        }
        if (connect(d_rotctld_socket, servinfop->ai_addr, servinfop->ai_addrlen) == -1) {
	        close(d_rotctld_socket);
	        continue;
        }

        break;
      }
      if (servinfop == NULL) {
        return;
      }
      freeaddrinfo(servinfo);
      d_rotctld_connected = true;
    }

    /**
     * Obtain all output ready for reading from rotctld to the first '\n'.
     *
     * This function is obtained from flyby (https://github.com/la1k/flyby).
     **/
    int sock_readline(int sockd, char *message, size_t bufsize)
    {
      int len=0, pos=0;
      char c='\0';

      if (message!=NULL) {
        message[bufsize-1]='\0';
      }

      do {
        len = recv(sockd, &c, 1, MSG_WAITALL);
        if (len <= 0) {
          break;
        }
        if (message!=NULL) {
          message[pos]=c;
          message[pos+1]='\0';
        }
        pos+=len;
      } while (c!='\n' && pos<bufsize-2);

      return pos;
    }

    void antenna_angle_tagger_impl::rotctld_read()
    {
      if (d_rotctld_connected) {
        //ask for current position
        send(d_rotctld_socket, "p\n", 2, 0);

        //read azimuth
        const int BUFFER_LEN = 256;
        char message[BUFFER_LEN];
        sock_readline(d_rotctld_socket, message, BUFFER_LEN);
        d_azimuth = strtod(message, NULL);

        //read elevation
        sock_readline(d_rotctld_socket, message, 255);
        d_elevation = strtod(message, NULL);
      }
    }

    int
    antenna_angle_tagger_impl::work (int noutput_items, gr_vector_const_void_star &input_items, gr_vector_void_star &output_items)
    {
      const float *in = (const float *) input_items[0];
      float *out = (float *) output_items[0];

      //pass all input to output
      memcpy(out, in, sizeof(float)*noutput_items);

      //get azimuth
      float prev_azi = d_azimuth;
      rotctld_read();
      if (round(d_azimuth) != prev_azi) {
        //tag stream by current azimuth if the new azimuth differ from the previous
        //at a precision of 1 degree
        pmt::pmt_t tag_key = pmt::string_to_symbol(ANTENNA_ANGLE_TAG);
        pmt::pmt_t tag_value = pmt::from_float(d_azimuth);
        add_item_tag(0, nitems_written(0), tag_key, tag_value);
      }

      return noutput_items;
    }

  } /* namespace antennapattern */
} /* namespace gr */


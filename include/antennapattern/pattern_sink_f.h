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


#ifndef INCLUDED_ANTENNAPATTERN_PATTERN_SINK_F_H
#define INCLUDED_ANTENNAPATTERN_PATTERN_SINK_F_H

#include <antennapattern/api.h>
#include <gnuradio/sync_block.h>

namespace gr {
  namespace antennapattern {

    /*!
     * \brief Collects mean, standard deviation, ... of the input signal and assigns to corresponding azimuth angles obtained from tags.
     * \ingroup antennapattern
     *
     * \details
     * In order for the azimuth angle collection functionality to work
     * properly, antenna_angle_tagger should be placed on the stream before the
     * input port of this block. The block will otherwise collect the signal
     * statistics in the azimuth angle 0. Will write to the defined output
     * filename when the block is stopped.
     */
    class ANTENNAPATTERN_API pattern_sink_f : virtual public gr::sync_block
    {
     public:
      typedef boost::shared_ptr<pattern_sink_f> sptr;

      /*!
       * \brief Return a shared_ptr to a new instance of antennapattern::pattern_sink_f.
       *
       * \param filename Output filename for antenna pattern
       */
      static sptr make(std::string filename);
    };

  } // namespace antennapattern
} // namespace gr

#endif /* INCLUDED_ANTENNAPATTERN_PATTERN_SINK_F_H */


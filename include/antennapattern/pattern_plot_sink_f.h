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


#ifndef INCLUDED_ANTENNAPATTERN_PATTERN_PLOT_SINK_F_H
#define INCLUDED_ANTENNAPATTERN_PATTERN_PLOT_SINK_F_H

#include <antennapattern/api.h>
#include <gnuradio/sync_block.h>

namespace gr {
  namespace antennapattern {
    /*!
     * \brief <+description of block+>
     * \ingroup antennapattern
     *
     */
    class ANTENNAPATTERN_API pattern_plot_sink_f : virtual public gr::sync_block
    {
     public:
      typedef boost::shared_ptr<pattern_plot_sink_f> sptr;

      /*!
       * \brief Return a shared_ptr to a new instance of antennapattern::pattern_plot_sink_f.
       *
       * To avoid accidental use of raw pointers, antennapattern::pattern_plot_sink_f's
       * constructor is in a private implementation
       * class. antennapattern::pattern_plot_sink_f::make is the public interface for
       * creating new instances.
       */
      static sptr make();
    };

  } // namespace antennapattern
} // namespace gr

#endif /* INCLUDED_ANTENNAPATTERN_PATTERN_PLOT_SINK_F_H */


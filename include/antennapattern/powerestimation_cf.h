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


#ifndef INCLUDED_ANTENNAPATTERN_POWERESTIMATION_CF_H
#define INCLUDED_ANTENNAPATTERN_POWERESTIMATION_CF_H

#include <antennapattern/api.h>
#include <gnuradio/block.h>

namespace gr {
  namespace antennapattern {

    /*!
     * \brief Tracks the statistics of the high and low level of the input signal,
     * and outputs the power of the current input sample if it is classified
     * as belonging to the high level of the signal.
     * \ingroup antennapattern
     *
     * \details
     * Starts output when the difference between high and low level is deemed
     * to be large enough by use of a T test.  Since probably only the low
     * level is properly gaussian distributed, and the high level might have
     * trends that will not be gaussian, it is adviced to start the block when
     * the signal is turned off, since it otherwise might try to characterize
     * two levels within the high level.  However, this will smooth itself out
     * after a while.
     */
    class ANTENNAPATTERN_API powerestimation_cf : virtual public gr::block
    {
     public:
      typedef boost::shared_ptr<powerestimation_cf> sptr;

      /*!
       * \brief Return a shared_ptr to a new instance of antennapattern::powerestimation_cf.
       *
       */
      static sptr make();
    };

  } // namespace antennapattern
} // namespace gr

#endif /* INCLUDED_ANTENNAPATTERN_POWERESTIMATION_CF_H */


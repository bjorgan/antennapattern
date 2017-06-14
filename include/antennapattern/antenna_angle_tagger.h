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


#ifndef INCLUDED_ANTENNAPATTERN_ANTENNA_ANGLE_TAGGER_H
#define INCLUDED_ANTENNAPATTERN_ANTENNA_ANGLE_TAGGER_H

#include <antennapattern/api.h>
#include <gnuradio/sync_block.h>

namespace gr {
  namespace antennapattern {

    /*!
     * \brief Tags a stream with azimuth and elevation angles obtained from rotctl.
     * \ingroup antennapattern
     *
     * \details
     * Connects to rotctl on the default host (localhost) and default port
     * (4533) in order to extract azimuth and elevation angles.  Will currently
     * not output any particular errors on failure, so be sure to follow the
     * output of rotctld with many verbose flags.
     */
    class ANTENNAPATTERN_API antenna_angle_tagger : virtual public gr::sync_block
    {
     public:
      typedef boost::shared_ptr<antenna_angle_tagger> sptr;

      /*!
       * \brief Return a shared_ptr to a new instance of antennapattern::antenna_angle_tagger.
       *
       */
      static sptr make();
    };

  } // namespace antennapattern
} // namespace gr

#endif /* INCLUDED_ANTENNAPATTERN_ANTENNA_ANGLE_TAGGER_H */


#ifndef INCLUDED_ANTENNAPATTERN_ARTIFICIAL_ANGLE_TAGGER_H
#define INCLUDED_ANTENNAPATTERN_ARTIFICIAL_ANGLE_TAGGER_H

#include <antennapattern/api.h>
#include <gnuradio/sync_block.h>

namespace gr {
  namespace antennapattern {

    /*!
     * \brief Add artifical angle tags to stream.
     * \ingroup antennapattern
     *
     * \details
     * Tags stream with angles, like antenna_angle_tagger. Increments the angle every other n samples (input to make()).
     */
    class ANTENNAPATTERN_API artificial_angle_tagger : virtual public gr::sync_block
    {
     public:
      typedef boost::shared_ptr<artificial_angle_tagger> sptr;

      /*!
       * \brief Return a shared_ptr to a new instance of antennapattern::artificial_angle_tagger.
       *
       */
      static sptr make(long sample_increment);
    };

  } // namespace antennapattern
} // namespace gr

#endif /* INCLUDED_ANTENNAPATTERN_ARTIFICIAL_ANGLE_TAGGER_H */


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
     * Tags stream with angles, like antenna_angle_tagger, but by incrementing
     * the angle every other n samples instead of reading from rotctld.
     * Increments the angle so that the angle goes from the start_azimuth to
     * the end_azimuth during the expected number of samples (d_num_samples).
     * Tries to obtain the number of samples from metafile source tags
     * transmitted in the stream if the total expected number of samples is not
     * set.
     */
    class ANTENNAPATTERN_API artificial_angle_tagger : virtual public gr::sync_block
    {
     public:
      typedef boost::shared_ptr<artificial_angle_tagger> sptr;

      /*!
       * \brief Return a shared_ptr to a new instance of antennapattern::artificial_angle_tagger.
       *
       */
      static sptr make(double start_azimuth, double end_azimuth);

      /**
       * Set total expected number of samples, and calculate appropriate sample increment.
       **/
      void set_num_samples(long tot_num_samples);

      /**
       * Get expected number of samples.
       **/
      long num_samples(){return d_num_samples;};

      /**
       * Set start azimuth.
       **/
      void set_start_azimuth(double start_azimuth){d_start_azimuth = start_azimuth;};

      /**
       * Get start azimuth.
       **/
      double start_azimuth(){return d_start_azimuth;};

      /**
       * Set end azimuth.
       **/
      void set_end_azimuth(double end_azimuth){d_end_azimuth = end_azimuth;};

      /**
       * Get end azimuth.
       **/
      double end_azimuth(){return d_end_azimuth;};

      /**
       * Get sample increment.
       **/
      long sample_increment(){return d_sample_increment;};
     private:
      ///total expected number of samples
      long d_num_samples;
      ///start azimuth angle
      double d_start_azimuth;
      ///last azimuth angle to send
      double d_end_azimuth;
      ///increment azimuth by 1 degree every time we reach this number of samples. Calculated from start and end azimuth angle
      ///and the total expected number of samples.
      long d_sample_increment;
    };

  } // namespace antennapattern
} // namespace gr

#endif /* INCLUDED_ANTENNAPATTERN_ARTIFICIAL_ANGLE_TAGGER_H */


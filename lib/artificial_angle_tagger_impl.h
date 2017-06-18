#ifndef INCLUDED_ANTENNAPATTERN_ARTIFICIAL_ANGLE_TAGGER_IMPL_H
#define INCLUDED_ANTENNAPATTERN_ARTIFICIAL_ANGLE_TAGGER_IMPL_H

#include <antennapattern/artificial_angle_tagger.h>

namespace gr {
  namespace antennapattern {

    class artificial_angle_tagger_impl : public artificial_angle_tagger
    {
     private:
      ///current azimuth
      float d_azimuth;
      ///current elevation
      float d_elevation;
      ///increment azimuth by 1 degree every time we reach this number of samples
      long d_sample_increment;
      ///number of samples since last azimuth increment
      long d_samples_since_last_increment;

     public:
      artificial_angle_tagger_impl(long sample_increment);
      ~artificial_angle_tagger_impl();

      // Where all the action really happens
      int work(int noutput_items,
         gr_vector_const_void_star &input_items,
         gr_vector_void_star &output_items);
    };

  } // namespace antennapattern
} // namespace gr

#endif /* INCLUDED_ANTENNAPATTERN_ARTIFICIAL_ANGLE_TAGGER_IMPL_H */


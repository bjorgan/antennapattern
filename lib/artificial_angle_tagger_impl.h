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
      ///number of samples since last azimuth increment
      long d_samples_since_last_increment;
      ///whether we have received the meta file header information
      bool d_meta_header_received;

     public:
      artificial_angle_tagger_impl(double start_azimuth, double end_azimuth, long tot_num_samples);
      ~artificial_angle_tagger_impl();

      int work(int noutput_items,
         gr_vector_const_void_star &input_items,
         gr_vector_void_star &output_items);
    };

  } // namespace antennapattern
} // namespace gr

#endif /* INCLUDED_ANTENNAPATTERN_ARTIFICIAL_ANGLE_TAGGER_IMPL_H */


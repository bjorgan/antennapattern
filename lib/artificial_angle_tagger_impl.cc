#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <gnuradio/io_signature.h>
#include "artificial_angle_tagger_impl.h"
#include "antenna_angle_tagger_impl.h"

namespace gr {
  namespace antennapattern {

    artificial_angle_tagger::sptr
    artificial_angle_tagger::make(long sample_increment)
    {
      return gnuradio::get_initial_sptr
        (new artificial_angle_tagger_impl(sample_increment));
    }

    /*
     * The private constructor
     */
    artificial_angle_tagger_impl::artificial_angle_tagger_impl(long sample_increment)
      : gr::sync_block("artificial_angle_tagger",
              gr::io_signature::make(1, 1, sizeof(float)),
              gr::io_signature::make(1, 1, sizeof(float))), d_sample_increment(sample_increment), d_azimuth(0.0f), d_elevation(0.0f), d_samples_since_last_increment(0)
    {}

    /*
     * Our virtual destructor.
     */
    artificial_angle_tagger_impl::~artificial_angle_tagger_impl()
    {
    }

    int
    artificial_angle_tagger_impl::work(int noutput_items,
        gr_vector_const_void_star &input_items,
        gr_vector_void_star &output_items)
    {
      const float *in = (const float *) input_items[0];
      float *out = (float *) output_items[0];
      memcpy(out, in, sizeof(float)*noutput_items);

      //get number of times we should increment the azimuth angle
      d_samples_since_last_increment += noutput_items;
      long times_exceeded = d_samples_since_last_increment/d_sample_increment;
      if (times_exceeded > 0) {
        d_samples_since_last_increment = d_samples_since_last_increment % d_sample_increment;
      }

      //set azimuth angle changes at increment steps
      for (int i=0; i < times_exceeded; i++) {
        pmt::pmt_t tag_key = pmt::string_to_symbol(ANTENNA_ANGLE_TAG);
        pmt::pmt_t tag_value = pmt::from_float(d_azimuth + i);
        add_item_tag(0, nitems_written(0) + i*d_sample_increment, tag_key, tag_value);
      }
      d_azimuth += times_exceeded;

      // Tell runtime system how many output items we produced.
      return noutput_items;
    }

  } /* namespace antennapattern */
} /* namespace gr */


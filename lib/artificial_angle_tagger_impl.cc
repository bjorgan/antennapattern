#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <gnuradio/io_signature.h>
#include "artificial_angle_tagger_impl.h"
#include "antenna_angle_tagger_impl.h"

namespace gr {
  namespace antennapattern {

    artificial_angle_tagger::sptr
    artificial_angle_tagger::make(double start_azimuth, double end_azimuth, double tot_num_samples)
    {
      return gnuradio::get_initial_sptr
        (new artificial_angle_tagger_impl(start_azimuth, end_azimuth, tot_num_samples));
    }
    void artificial_angle_tagger::set_num_samples(double tot_num_samples) {
      d_num_samples = tot_num_samples;
      d_sample_increment = tot_num_samples/(d_end_azimuth - d_start_azimuth);
    }

    /*
     * The private constructor
     */
    artificial_angle_tagger_impl::artificial_angle_tagger_impl(double start_azimuth, double end_azimuth, long tot_num_samples)
      : gr::sync_block("artificial_angle_tagger",
              gr::io_signature::make(1, 1, sizeof(gr_complex)),
              gr::io_signature::make(1, 1, sizeof(gr_complex))), d_azimuth(0.0f), d_elevation(0.0f), d_samples_since_last_increment(0)
    {
      set_start_azimuth(start_azimuth);
      set_end_azimuth(end_azimuth);
      set_num_samples(tot_num_samples);
    }

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
      const gr_complex *in = (const gr_complex *) input_items[0];
      gr_complex *out = (gr_complex *) output_items[0];
      memcpy(out, in, sizeof(gr_complex)*noutput_items);

      if (num_samples() == 0) {
        throw std::runtime_error("Total number of samples not set. Set total number of samples so that angles are incremented correctly.");
      }

      //get number of times we should increment the azimuth angle
      d_samples_since_last_increment += noutput_items;
      long times_exceeded = d_samples_since_last_increment/sample_increment();
      if (times_exceeded > 0) {
        d_samples_since_last_increment = d_samples_since_last_increment % sample_increment();
      }

      //set azimuth angle changes at increment steps
      for (int i=0; i < times_exceeded; i++) {
        pmt::pmt_t tag_key = pmt::string_to_symbol(ANTENNA_ANGLE_TAG);
        pmt::pmt_t tag_value = pmt::from_float(d_azimuth + i);
        add_item_tag(0, nitems_written(0) + i*sample_increment(), tag_key, tag_value);
      }
      d_azimuth += times_exceeded;

      // Tell runtime system how many output items we produced.
      return noutput_items;
    }

  } /* namespace antennapattern */
} /* namespace gr */


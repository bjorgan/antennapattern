#!/usr/bin/env python
# -*- coding: utf-8 -*-

from gnuradio import gr, gr_unittest
from gnuradio import blocks
import antennapattern_swig as antennapattern
import numpy as np

class qa_artificial_angle_tagger (gr_unittest.TestCase):

    def setUp (self):
        self.tb = gr.top_block ()

    def tearDown (self):
        self.tb = None

    def test_001_t (self):
        num_samples = 6000;
        src_data = np.random.normal(0.5, 0.01, num_samples);
        start_degree = 0;
        end_degree = 180;

        #set up blocks
        src = blocks.vector_source_f(src_data);
        sample_increment = num_samples/(end_degree - start_degree);
        tagger = antennapattern.artificial_angle_tagger(start_degree, end_degree);
        tagger.set_num_samples(num_samples)
        dst = blocks.vector_sink_f();

        self.tb.connect(src, tagger);
        self.tb.connect(tagger, dst);

        self.tb.run ()

        #check that we get one tag for each angle
        tags = [gr.tag_to_python(x) for x in dst.tags()]
        self.assertEqual(end_degree - start_degree + 1, len(tags));
        expected_offset = 0;
        expected_angle = start_degree;
        for tag in tags:
            self.assertEqual(tag.offset, expected_offset);
            self.assertEqual(tag.value, expected_angle, 1);
            expected_offset += sample_increment;
            expected_angle += 1;


if __name__ == '__main__':
    gr_unittest.run(qa_artificial_angle_tagger, "qa_artificial_angle_tagger.xml")

#!/usr/bin/env python
# -*- coding: utf-8 -*-
#
# Copyright 2017 <+YOU OR YOUR COMPANY+>.
#
# This is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 3, or (at your option)
# any later version.
#
# This software is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this software; see the file COPYING.  If not, write to
# the Free Software Foundation, Inc., 51 Franklin Street,
# Boston, MA 02110-1301, USA.
#

from gnuradio import gr, gr_unittest
from gnuradio import blocks
import antennapattern_swig as antennapattern
import numpy as np

class qa_powerestimation_cf (gr_unittest.TestCase):

    def setUp (self):
        self.tb = gr.top_block ()

    def tearDown (self):
        self.tb = None

    def test_001_alternating_values_t (self):
        #set up vector of alternating, normally distributed values
        src_data = np.empty((500,))
        low_value = 0.1;
        high_value = 1.0;
        stdev = 0.01;
        src_data[::2] = np.random.normal(low_value, stdev, len(src_data[::2]));
        src_data[1::2] = np.random.normal(high_value, stdev, len(src_data[1::2]));
        expected_result = high_value-low_value;
        src = blocks.vector_source_c(src_data);

        #set up blocks
        powerestimation_block = antennapattern.powerestimation_cf();
        dst = blocks.vector_sink_f();

        self.tb.connect(src, powerestimation_block);
        self.tb.connect(powerestimation_block, dst);

        self.tb.run ()

        #check data
        result_data = dst.data()
        self.assertTrue(len(result_data) > 0)
        self.assertAlmostEqual(expected_result, np.mean(result_data), 1)

    def test_003_clustered_values_t (self):
        #set up vector of normally distributed values, with the 500 first being normally distributed around low level, and the next 500 around high level
        low_level = 0.1;
        high_level = 0.6;
        src_data = np.concatenate([np.random.normal(low_level, 0.01, 500), np.random.normal(high_level, 0.01, 500)], axis=0);
        expected_result = high_level - low_level;

        #set up blocks
        src = blocks.vector_source_c(src_data);
        dst = blocks.vector_sink_f();

        powerestimation_block = antennapattern.powerestimation_cf();

        self.tb.connect(src, powerestimation_block);
        self.tb.connect(powerestimation_block, dst);

        self.tb.run ()

        # check data
        result_data = dst.data()
        self.assertTrue(len(result_data) > 0)
        self.assertAlmostEqual(expected_result, np.mean(result_data), 1)

    def test_004_clustered_values_longrange_during_first_t (self):
        #do same as above, but let low values have a much longer duration than the high values
        mean_1 = 0.1;
        mean_2 = 0.6;
        src_data = np.concatenate([np.random.normal(mean_1, 0.01, 500000), np.random.normal(mean_2, 0.01, 500)], axis=0);
        expected_result = mean_2 - mean_1;

        #set up blocks
        src = blocks.vector_source_c(src_data);
        dst = blocks.vector_sink_f();

        powerestimation_block = antennapattern.powerestimation_cf();

        self.tb.connect(src, powerestimation_block);
        self.tb.connect(powerestimation_block, dst);

        self.tb.run ()

        # check data
        result_data = dst.data()
        self.assertTrue(len(result_data) > 0)
        self.assertAlmostEqual(expected_result, np.mean(result_data), 1)


if __name__ == '__main__':
    gr_unittest.run(qa_powerestimation_cf, "qa_powerestimation_cf.xml")

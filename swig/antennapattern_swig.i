/* -*- c++ -*- */

#define ANTENNAPATTERN_API

%include "gnuradio.i"			// the common stuff

//load generated python docstrings
%include "antennapattern_swig_doc.i"

%{
#include "antennapattern/powerestimation_cf.h"
#include "antennapattern/antenna_angle_tagger.h"
#include "antennapattern/pattern_sink_f.h"
#include "antennapattern/artificial_angle_tagger.h"
%}


%include "antennapattern/powerestimation_cf.h"
GR_SWIG_BLOCK_MAGIC2(antennapattern, powerestimation_cf);
%include "antennapattern/antenna_angle_tagger.h"
GR_SWIG_BLOCK_MAGIC2(antennapattern, antenna_angle_tagger);
%include "antennapattern/pattern_sink_f.h"
GR_SWIG_BLOCK_MAGIC2(antennapattern, pattern_sink_f);
%include "antennapattern/artificial_angle_tagger.h"
GR_SWIG_BLOCK_MAGIC2(antennapattern, artificial_angle_tagger);

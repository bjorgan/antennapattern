# Copyright 2011,2012,2016 Free Software Foundation, Inc.
#
# This file is part of GNU Radio
#
# GNU Radio is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 3, or (at your option)
# any later version.
#
# GNU Radio is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with GNU Radio; see the file COPYING.  If not, write to
# the Free Software Foundation, Inc., 51 Franklin Street,
# Boston, MA 02110-1301, USA.

########################################################################
# Qt
########################################################################

set(qtgui_moc_hdrs polar_diagram_widget.h)
QT4_WRAP_CPP(qtgui_moc_sources ${qtgui_moc_hdrs})

########################################################################
# Setup library
########################################################################
include(GrPlatform) #define LIB_SUFFIX

add_definitions(${QT_DEFINITIONS})
include(${QT_USE_FILE})
include_directories(${Boost_INCLUDE_DIR}
        ${QT_INCLUDE_DIRS}
        ${QWT_INCLUDE_DIRS})
link_directories(${Boost_LIBRARY_DIRS}
        ${QWT_LIBRARY_DIRS})
list(APPEND antennapattern_sources
    powerestimation_cf_impl.cc
    antenna_angle_tagger_impl.cc
    pattern_sink_f_impl.cc
    artificial_angle_tagger_impl.cc
    pattern_plot_sink_f_impl.cc 
    ${qtgui_moc_sources})

set(antennapattern_sources "${antennapattern_sources}" PARENT_SCOPE)
if(NOT antennapattern_sources)
	MESSAGE(STATUS "No C++ sources... skipping lib/")
	return()
endif(NOT antennapattern_sources)

add_library(gnuradio-antennapattern SHARED ${antennapattern_sources})
target_link_libraries(gnuradio-antennapattern ${Boost_LIBRARIES} ${GNURADIO_ALL_LIBRARIES} ${QT_LIBRARIES} ${QWT_LIBRARIES})
set_target_properties(gnuradio-antennapattern PROPERTIES DEFINE_SYMBOL "gnuradio_antennapattern_EXPORTS")

if(APPLE)
    set_target_properties(gnuradio-antennapattern PROPERTIES
        INSTALL_NAME_DIR "${CMAKE_INSTALL_PREFIX}/lib"
    )
endif(APPLE)

########################################################################
# Install built library files
########################################################################
include(GrMiscUtils)
GR_LIBRARY_FOO(gnuradio-antennapattern RUNTIME_COMPONENT "antennapattern_runtime" DEVEL_COMPONENT "antennapattern_devel")

########################################################################
# Build and register unit test
########################################################################
include(GrTest)

include_directories(${CPPUNIT_INCLUDE_DIRS})

list(APPEND test_antennapattern_sources
    ${CMAKE_CURRENT_SOURCE_DIR}/test_antennapattern.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/qa_antennapattern.cc
)

add_executable(test-antennapattern ${test_antennapattern_sources})

target_link_libraries(
  test-antennapattern
  ${GNURADIO_RUNTIME_LIBRARIES}
  ${Boost_LIBRARIES}
  ${CPPUNIT_LIBRARIES}
  gnuradio-antennapattern
)

GR_ADD_TEST(test_antennapattern test-antennapattern)

########################################################################
# Print summary
########################################################################
message(STATUS "Using install prefix: ${CMAKE_INSTALL_PREFIX}")
message(STATUS "Building for version: ${VERSION} / ${LIBVER}")


/* -*- c++ -*- */
/* 
 * Copyright 2017 <+YOU OR YOUR COMPANY+>.
 * 
 * This is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3, or (at your option)
 * any later version.
 * 
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this software; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street,
 * Boston, MA 02110-1301, USA.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <gnuradio/io_signature.h>
#include "pattern_plot_sink_f_impl.h"
#include "antenna_angle_tagger_impl.h"
#include "polar_diagram_widget.h"
#include <QGridLayout>

#include <qwt-qt4/qwt_plot.h>
#include <qwt-qt4/qwt_plot_curve.h>
#include <qwt-qt4/qwt_legend.h>
#include <qwt-qt4/qwt_symbol.h>
#include <qwt-qt4/qwt_plot_marker.h>

namespace gr {
  namespace antennapattern {
    polar_diagram_widget::polar_diagram_widget(QWidget *parent) : QWidget(parent)
    {
      plot = new QwtPlot(this);

      QGridLayout *layout = new QGridLayout(this);
      layout->addWidget(plot, 0, 0);

      QwtPlotCurve *curve = new QwtPlotCurve;
      curve->attach(plot);
    }
    
    void polar_diagram_widget::update_plot(QVector<double> angles, QVector<double> magnitudes)
    {
      QVector<double> x_values, y_values;
      for (int i=0; i < angles.size(); i++) {
        x_values.push_back(magnitudes[i]*cos(angles[i]/180.0*M_PI));
        y_values.push_back(magnitudes[i]*sin(angles[i]/180.0*M_PI));
      }
      curve->setData(x_values, y_values);
      plot->replot();
    }

    pattern_plot_sink_f::sptr
    pattern_plot_sink_f::make()
    {
      return gnuradio::get_initial_sptr
        (new pattern_plot_sink_f_impl());
    }

    /*
     * The private constructor
     */
    pattern_plot_sink_f_impl::pattern_plot_sink_f_impl()
      : gr::sync_block("pattern_plot_sink_f",
              gr::io_signature::make(1, 1, sizeof(float)),
              gr::io_signature::make(0, 0, 0))
    {}

    /*
     * Our virtual destructor.
     */
    pattern_plot_sink_f_impl::~pattern_plot_sink_f_impl()
    {
    }

    int
    pattern_plot_sink_f_impl::work(int noutput_items,
        gr_vector_const_void_star &input_items,
        gr_vector_void_star &output_items)
    {
      const float *in = (const float *) input_items[0];

      //obtain the azimuth tags during the time window
      std::vector<gr::tag_t> tags;
      get_tags_in_range(tags, 0, nitems_read(0), nitems_read(0) + noutput_items, pmt::mp(ANTENNA_ANGLE_TAG));

      d_mapped_values.add_samples(tags, nitems_read(0), noutput_items, in);

      return noutput_items;
    }

  } /* namespace antennapattern */
} /* namespace gr */


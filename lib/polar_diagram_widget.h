#include <QWidget>

class QwtPlot;
class QwtPlotCurve;
class QwtPlotMarker;

namespace gr {
  namespace antennapattern {

    /**
    * Polar diagram widget.
    **/
    class polar_diagram_widget : public QWidget{
    Q_OBJECT
    public:
     polar_diagram_widget(QWidget *parent = NULL);
    public slots:
     void update_plot(QVector<double> angles, QVector<double> magnitudes);
    public:
     void update_plot_from_external(QVector<double> angles, QVector<double> magnitudes);
    signals:
     void should_update_plot(QVector<double> angles, QVector<double> magnitudes);
    private:
      QwtPlot *plot;
      QwtPlotCurve* curve;
    };

  }
}

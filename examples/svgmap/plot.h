#include <qwt_plot.h>

class QwtPlotSvgItem;

class Plot : public QwtPlot
{
    Q_OBJECT

public:
    Plot(QWidget * = NULL);

public Q_SLOTS:
    void loadSVG();

private:
    void rescale();

    QwtPlotSvgItem *d_mapItem;
    const QRectF d_mapRect;
};

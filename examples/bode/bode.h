#include <qmainwindow.h>

class QwtPlotZoomer;
class QwtPlotPicker;
class QwtPlotPanner;
class BodePlot;

class MainWin : public QMainWindow
{
    Q_OBJECT

public:
    MainWin(QWidget *parent = 0);

private Q_SLOTS:
    void moved(const QPoint &);
    void selected(const QPolygon &);

    void print();
    void exportSVG();
    void enableZoomMode(bool);

private:
    void showInfo(QString text = QString());

    BodePlot *d_plot;

    QwtPlotZoomer *d_zoomer[2];
    QwtPlotPicker *d_picker;
    QwtPlotPanner *d_panner;
};

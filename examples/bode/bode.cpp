#include <qregexp.h>
#include <qapplication.h>
#include <qtoolbar.h>
#include <qtoolbutton.h>
#include <qlabel.h>
#include <qlayout.h>
#include <qstatusbar.h>
#include <qprinter.h>
#include <qpicture.h>
#include <qpainter.h>
#include <qfiledialog.h>
#ifdef QT_SVG_LIB
#include <qsvggenerator.h>
#endif
#include <qprintdialog.h>
#include <qfileinfo.h>
#include <qwt_counter.h>
#include <qwt_plot_zoomer.h>
#include <qwt_plot_panner.h>
#include <qwt_text.h>
#include <qwt_math.h>
#include "pixmaps.h"
#include "bode_plot.h"
#include "bode.h"

class Zoomer : public QwtPlotZoomer
{
public:
    Zoomer(int xAxis, int yAxis, QwtPlotCanvas *canvas) : QwtPlotZoomer(xAxis, yAxis, canvas)
    {
        setSelectionFlags(QwtPicker::DragSelection | QwtPicker::CornerToCorner);
        setTrackerMode(QwtPicker::AlwaysOff);
        setRubberBand(QwtPicker::NoRubberBand);

        // RightButton: zoom out by 1
        // Ctrl+RightButton: zoom out to full size

        setMousePattern(QwtEventPattern::MouseSelect2, Qt::RightButton, Qt::ControlModifier);
        setMousePattern(QwtEventPattern::MouseSelect3, Qt::RightButton);
    }
};

//-----------------------------------------------------------------
//
//      bode.cpp -- A demo program featuring QwtPlot and QwtCounter
//
//      This example demonstrates the mapping of different curves
//      to different axes in a QwtPlot widget. It also shows how to
//      display the cursor position and how to implement zooming.
//
//-----------------------------------------------------------------

MainWin::MainWin(QWidget *parent) : QMainWindow(parent)
{
    d_plot = new BodePlot(this);
    d_plot->setMargin(5);

    setContextMenuPolicy(Qt::NoContextMenu);

    d_zoomer[0] = new Zoomer(QwtPlot::xBottom, QwtPlot::yLeft, d_plot->canvas());
    d_zoomer[0]->setRubberBand(QwtPicker::RectRubberBand);
    d_zoomer[0]->setRubberBandPen(QColor(Qt::green));
    d_zoomer[0]->setTrackerMode(QwtPicker::ActiveOnly);
    d_zoomer[0]->setTrackerPen(QColor(Qt::white));

    d_zoomer[1] = new Zoomer(QwtPlot::xTop, QwtPlot::yRight, d_plot->canvas());

    d_panner = new QwtPlotPanner(d_plot->canvas());
    d_panner->setMouseButton(Qt::MiddleButton);

    d_picker = new QwtPlotPicker(
            QwtPlot::xBottom, QwtPlot::yLeft, QwtPicker::PointSelection | QwtPicker::DragSelection,
            QwtPlotPicker::CrossRubberBand, QwtPicker::AlwaysOn, d_plot->canvas());
    d_picker->setRubberBandPen(QColor(Qt::green));
    d_picker->setRubberBand(QwtPicker::CrossRubberBand);
    d_picker->setTrackerPen(QColor(Qt::white));

    setCentralWidget(d_plot);

    QToolBar *toolBar = new QToolBar(this);

    QToolButton *btnZoom = new QToolButton(toolBar);
    btnZoom->setText("Zoom");
    btnZoom->setIcon(QIcon(QPixmap(zoom_xpm)));
    btnZoom->setCheckable(true);
    btnZoom->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

    QToolButton *btnPrint = new QToolButton(toolBar);
    btnPrint->setText("Print");
    btnPrint->setIcon(QIcon(QPixmap(print_xpm)));
    btnPrint->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

#ifdef QT_SVG_LIB
    QToolButton *btnSVG = new QToolButton(toolBar);
    btnSVG->setText("SVG");
    btnSVG->setIcon(QIcon(QPixmap(print_xpm)));
    btnSVG->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
#endif

    toolBar->addWidget(btnZoom);
    toolBar->addWidget(btnPrint);
#ifdef QT_SVG_LIB
    toolBar->addWidget(btnSVG);
#endif
    toolBar->addSeparator();

    QWidget *hBox = new QWidget(toolBar);

    QHBoxLayout *layout = new QHBoxLayout(hBox);
    layout->setSpacing(0);
    layout->addWidget(new QWidget(hBox), 10); // spacer
    layout->addWidget(new QLabel("Damping Factor", hBox), 0);
    layout->addSpacing(10);

    QwtCounter *cntDamp = new QwtCounter(hBox);
    cntDamp->setRange(0.0, 5.0, 0.01);
    cntDamp->setValue(0.0);

    layout->addWidget(cntDamp, 0);

    (void)toolBar->addWidget(hBox);

    addToolBar(toolBar);
#ifndef QT_NO_STATUSBAR
    (void)statusBar();
#endif

    enableZoomMode(false);
    showInfo();

    connect(cntDamp, SIGNAL(valueChanged(double)), d_plot, SLOT(setDamp(double)));

    connect(btnPrint, SIGNAL(clicked()), SLOT(print()));
#ifdef QT_SVG_LIB
    connect(btnSVG, SIGNAL(clicked()), SLOT(exportSVG()));
#endif
    connect(btnZoom, SIGNAL(toggled(bool)), SLOT(enableZoomMode(bool)));

    connect(d_picker, SIGNAL(moved(const QPoint &)), SLOT(moved(const QPoint &)));
    connect(d_picker, SIGNAL(selected(const QPolygon &)), SLOT(selected(const QPolygon &)));
}

void MainWin::print()
{
    QPrinter printer;

    QString docName = d_plot->title().text();
    if (!docName.isEmpty()) {
        docName.replace(QRegExp(QString::fromLatin1("\n")), tr(" -- "));
        printer.setDocName(docName);
    }

    printer.setCreator("Bode example");
    printer.setPageOrientation(QPageLayout::Landscape);

    QPrintDialog dialog(&printer);
    if (dialog.exec()) {
        QwtPlotPrintFilter filter;
        if (printer.colorMode() == QPrinter::GrayScale) {
            int options = QwtPlotPrintFilter::PrintAll;
            options &= ~QwtPlotPrintFilter::PrintBackground;
            options |= QwtPlotPrintFilter::PrintFrameWithScales;
            filter.setOptions(options);
        }
        d_plot->print(printer, filter);
    }
}

void MainWin::exportSVG()
{
    QString fileName = "bode.svg";

#ifdef QT_SVG_LIB
#ifndef QT_NO_FILEDIALOG
    fileName = QFileDialog::getSaveFileName(this, "Export File Name", QString(),
                                            "SVG Documents (*.svg)");
#endif
    if (!fileName.isEmpty()) {
        QSvgGenerator generator;
        generator.setFileName(fileName);
        generator.setSize(QSize(800, 600));

        d_plot->print(generator);
    }
#endif
}

void MainWin::enableZoomMode(bool on)
{
    d_panner->setEnabled(on);

    d_zoomer[0]->setEnabled(on);
    d_zoomer[0]->zoom(0);

    d_zoomer[1]->setEnabled(on);
    d_zoomer[1]->zoom(0);

    d_picker->setEnabled(!on);

    showInfo();
}

void MainWin::showInfo(QString text)
{
    if (text == QString()) {
        if (d_picker->rubberBand())
            text = "Cursor Pos: Press left mouse button in plot region";
        else
            text = "Zoom: Press mouse button and drag";
    }

#ifndef QT_NO_STATUSBAR
    statusBar()->showMessage(text);
#endif
}

void MainWin::moved(const QPoint &pos)
{
    QString info;
    info = "Freq=" + QString::number(d_plot->invTransform(QwtPlot::xBottom, pos.x()), 'g')
            + ", Ampl=" + QString::number(d_plot->invTransform(QwtPlot::yLeft, pos.y()), 'g')
            + ", Phase=" + QString::number(d_plot->invTransform(QwtPlot::yRight, pos.y()), 'g');
    showInfo(info);
}

void MainWin::selected(const QPolygon &)
{
    showInfo();
}

int main(int argc, char **argv)
{
    QApplication a(argc, argv);

    MainWin w;
    w.resize(540, 400);
    w.show();

    int rv = a.exec();
    return rv;
}

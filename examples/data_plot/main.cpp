#include <qapplication.h>
#include <qmainwindow.h>
#include <qwt_counter.h>
#include <qtoolbar.h>
#include <qlabel.h>
#include <qlayout.h>
#include "data_plot.h"

class MainWindow: public QMainWindow
{
public:
    MainWindow()
    {
        QToolBar *toolBar = new QToolBar(this);
        toolBar->setFixedHeight(80);

        toolBar->setAllowedAreas(Qt::TopToolBarArea | Qt::BottomToolBarArea);
        QWidget *hBox = new QWidget(toolBar);
        QLabel *label = new QLabel("Timer Interval", hBox);
        QwtCounter *counter = new QwtCounter(hBox);
        counter->setRange(-1.0, 100.0, 1.0);

        QHBoxLayout *layout = new QHBoxLayout(hBox);
        layout->addWidget(label);
        layout->addWidget(counter);
        layout->addWidget(new QWidget(hBox), 10); // spacer);

        toolBar->addWidget(hBox);
        addToolBar(toolBar);


        DataPlot *plot = new DataPlot(this);
        setCentralWidget(plot);

        connect(counter, SIGNAL(valueChanged(double)),
            plot, SLOT(setTimerInterval(double)) );

        counter->setValue(20.0);
    }
};

int main(int argc, char **argv)
{
    QApplication a(argc, argv);

    MainWindow mainWindow;

    mainWindow.resize(600,400);
    mainWindow.show();

    return a.exec(); 
}

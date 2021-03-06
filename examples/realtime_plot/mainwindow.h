#ifndef _MAINWINDOW_H_
#define _MAINWINDOW_H_ 1

#include <qmainwindow.h>
#include <qaction.h>

class QSpinBox;
class QPushButton;
class RandomPlot;
class Counter;

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow();

private Q_SLOTS:
    void showRunning(bool);
    void appendPoints(bool);

private:
    QToolBar *toolBar();
    void initWhatsThis();

private:
    Counter *d_randomCount;
    Counter *d_timerCount;
    QAction *d_startAction;
    QAction *d_clearAction;

    RandomPlot *d_plot;
};

#endif

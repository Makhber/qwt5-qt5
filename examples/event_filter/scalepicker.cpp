#include <qevent.h>
#include <qwt_plot.h>
#include <qwt_scale_widget.h>
#include "scalepicker.h"

ScalePicker::ScalePicker(QwtPlot *plot) : QObject(plot)
{
    for (uint i = 0; i < QwtPlot::axisCnt; i++) {
        QwtScaleWidget *scaleWidget = (QwtScaleWidget *)plot->axisWidget(i);
        if (scaleWidget)
            scaleWidget->installEventFilter(this);
    }
}

bool ScalePicker::eventFilter(QObject *object, QEvent *e)
{
    if (object->inherits("QwtScaleWidget") && e->type() == QEvent::MouseButtonPress) {
        mouseClicked((const QwtScaleWidget *)object, ((QMouseEvent *)e)->pos());
        return true;
    }

    return QObject::eventFilter(object, e);
}

void ScalePicker::mouseClicked(const QwtScaleWidget *scale, const QPoint &pos)
{
    QRect rect = scaleRect(scale);

    int margin = 10; // 10 pixels tolerance
    rect.setRect(rect.x() - margin, rect.y() - margin, rect.width() + 2 * margin,
                 rect.height() + 2 * margin);

    if (rect.contains(pos)) // No click on the title
    {
        // translate the position in a value on the scale

        double value = 0.0;
        int axis = -1;

        const QwtScaleDraw *sd = scale->scaleDraw();
        switch (scale->alignment()) {
        case QwtScaleDraw::LeftScale: {
            value = sd->map().invTransform(pos.y());
            axis = QwtPlot::yLeft;
            break;
        }
        case QwtScaleDraw::RightScale: {
            value = sd->map().invTransform(pos.y());
            axis = QwtPlot::yRight;
            break;
        }
        case QwtScaleDraw::BottomScale: {
            value = sd->map().invTransform(pos.x());
            axis = QwtPlot::xBottom;
            break;
        }
        case QwtScaleDraw::TopScale: {
            value = sd->map().invTransform(pos.x());
            axis = QwtPlot::xTop;
            break;
        }
        }
        Q_EMIT clicked(axis, value);
    }
}

// The rect of a scale without the title
QRect ScalePicker::scaleRect(const QwtScaleWidget *scale) const
{
    const int bld = scale->margin();
    const int mjt = scale->scaleDraw()->majTickLength();
    const int sbd = scale->startBorderDist();
    const int ebd = scale->endBorderDist();

    QRect rect;
    switch (scale->alignment()) {
    case QwtScaleDraw::LeftScale: {
        rect.setRect(scale->width() - bld - mjt, sbd, mjt, scale->height() - sbd - ebd);
        break;
    }
    case QwtScaleDraw::RightScale: {
        rect.setRect(bld, sbd, mjt, scale->height() - sbd - ebd);
        break;
    }
    case QwtScaleDraw::BottomScale: {
        rect.setRect(sbd, bld, scale->width() - sbd - ebd, mjt);
        break;
    }
    case QwtScaleDraw::TopScale: {
        rect.setRect(sbd, scale->height() - bld - mjt, scale->width() - sbd - ebd, mjt);
        break;
    }
    }
    return rect;
}

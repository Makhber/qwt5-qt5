/* -*- mode: C++ ; c-file-style: "stroustrup" -*- *****************************
 * Qwt Widget Library
 * Copyright (C) 1997   Josef Wilgen
 * Copyright (C) 2002   Uwe Rathmann
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the Qwt License, Version 1.0
 *****************************************************************************/

// vim: expandtab

#ifndef QWT_PLOT_PICKER_H
#define QWT_PLOT_PICKER_H

#include "qwt_plot_canvas.h"
#include "qwt_picker.h"

class QwtPlot;

/*!
  \brief QwtPlotPicker provides selections on a plot canvas

  QwtPlotPicker is a QwtPicker tailored for selections on
  a plot canvas. It is set to a x-Axis and y-Axis and
  translates all pixel coordinates into this coodinate system.
*/

class QWT_EXPORT QwtPlotPicker : public QwtPicker
{
    Q_OBJECT

public:
    explicit QwtPlotPicker(QwtPlotCanvas *);
    virtual ~QwtPlotPicker();

    explicit QwtPlotPicker(int xAxis, int yAxis, QwtPlotCanvas *);

    explicit QwtPlotPicker(int xAxis, int yAxis, int selectionFlags, RubberBand rubberBand,
                           DisplayMode trackerMode, QwtPlotCanvas *);

    virtual void setAxis(int xAxis, int yAxis);

    int xAxis() const;
    int yAxis() const;

    QwtPlot *plot();
    const QwtPlot *plot() const;

    QwtPlotCanvas *canvas();
    const QwtPlotCanvas *canvas() const;

Q_SIGNALS:

    /*!
      A signal emitted in case of selectionFlags() & PointSelection.
      \param pos Selected point
    */
    void selected(const QPointF &pos);

    /*!
      A signal emitted in case of selectionFlags() & RectSelection.
      \param rect Selected rectangle
    */
    void selected(const QRectF &rect);

    /*!
      A signal emitting the selected points,
      at the end of a selection.

      \param pa Selected points
    */
    void selected(const QVector<QPointF> &pa);

    /*!
      A signal emitted when a point has been appended to the selection

      \param pos Position of the appended point.
      \sa append(). moved()
    */
    void appended(const QPointF &pos);

    /*!
      A signal emitted whenever the last appended point of the
      selection has been moved.

      \param pos Position of the moved last point of the selection.
      \sa move(), appended()
    */
    void moved(const QPointF &pos);

protected:
    QRectF scaleRect() const;

    QRectF invTransform(const QRect &) const;
    QRect transform(const QRectF &) const;

    QPointF invTransform(const QPoint &) const;
    QPoint transform(const QPointF &) const;

    virtual QwtText trackerText(const QPoint &) const;
    virtual QwtText trackerText(const QPointF &) const;

    virtual void move(const QPoint &);
    virtual void append(const QPoint &);
    virtual bool end(bool ok = true);

private:
    int d_xAxis;
    int d_yAxis;
};

#endif

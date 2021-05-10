/* -*- mode: C++ ; c-file-style: "stroustrup" -*- *****************************
 * Qwt Widget Library
 * Copyright (C) 1997   Josef Wilgen
 * Copyright (C) 2002   Uwe Rathmann
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the Qwt License, Version 1.0
 *****************************************************************************/

// vim: expandtab

#ifndef QWT_DATA_H
#define QWT_DATA_H 1

#include "qwt_global.h"

#include <QPolygonF>

// MOC_SKIP_BEGIN

#if defined(QWT_TEMPLATEDLL)

template class QWT_EXPORT QVector<double>;

#endif

// MOC_SKIP_END

/*!
  \brief QwtData defines an interface to any type of curve data.

  Classes, derived from QwtData may:
  - store the data in almost any type of container
  - calculate the data on the fly instead of storing it
 */

class QWT_EXPORT QwtData
{
public:
    QwtData();
    virtual ~QwtData();

    //! \return Pointer to a copy (virtual copy constructor)
    virtual QwtData *copy() const = 0;

    //! \return Size of the data set
    virtual size_t size() const = 0;

    /*!
      Return the x value of data point i
      \param i Index
      \return x X value of data point i
     */
    virtual double x(size_t i) const = 0;
    /*!
      Return the y value of data point i
      \param i Index
      \return y Y value of data point i
     */
    virtual double y(size_t i) const = 0;

    virtual QRectF boundingRect() const;

protected:
    /*!
      Assignment operator (virtualized)
     */
    QwtData &operator=(const QwtData &);
};

/*!
  \brief Data class containing a single QVector<QPointF> object.
 */
class QWT_EXPORT QwtPolygonFData : public QwtData
{
public:
    QwtPolygonFData(const QPolygonF &);

    QwtPolygonFData &operator=(const QwtPolygonFData &);
    virtual QwtData *copy() const;

    virtual size_t size() const;
    virtual double x(size_t i) const;
    virtual double y(size_t i) const;

    const QPolygonF &data() const;

private:
    QPolygonF d_data;
};

/*!
  \brief Data class containing two QVector<double> objects.
 */

class QWT_EXPORT QwtArrayData : public QwtData
{
public:
    QwtArrayData(const QVector<double> &x, const QVector<double> &y);
    QwtArrayData(const double *x, const double *y, size_t size);
    QwtArrayData &operator=(const QwtArrayData &);
    virtual QwtData *copy() const;

    virtual size_t size() const;
    virtual double x(size_t i) const;
    virtual double y(size_t i) const;

    const QVector<double> &xData() const;
    const QVector<double> &yData() const;

    virtual QRectF boundingRect() const;

private:
    QVector<double> d_x;
    QVector<double> d_y;
};

/*!
  \brief Data class containing two pointers to memory blocks of doubles.
 */
class QWT_EXPORT QwtCPointerData : public QwtData
{
public:
    QwtCPointerData(const double *x, const double *y, size_t size);
    QwtCPointerData &operator=(const QwtCPointerData &);
    virtual QwtData *copy() const;

    virtual size_t size() const;
    virtual double x(size_t i) const;
    virtual double y(size_t i) const;

    const double *xData() const;
    const double *yData() const;

    virtual QRectF boundingRect() const;

private:
    const double *d_x;
    const double *d_y;
    size_t d_size;
};

#endif // !QWT_DATA

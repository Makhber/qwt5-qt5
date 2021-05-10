/* -*- mode: C++ ; c-file-style: "stroustrup" -*- *****************************
 * Qwt Widget Library
 * Copyright (C) 1997   Josef Wilgen
 * Copyright (C) 2002   Uwe Rathmann
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the Qwt License, Version 1.0
 *****************************************************************************/

#ifndef QWT_CURVE_FITTER_H
#define QWT_CURVE_FITTER_H

#include "qwt_global.h"

class QwtSpline;

#include <QPolygonF>

/*!
  \brief Abstract base class for a curve fitter
*/
class QWT_EXPORT QwtCurveFitter
{
public:
    virtual ~QwtCurveFitter();

    /*!
        Find a curve which has the best fit to a series of data points

        \param polygon Series of data points
        \return Curve points
     */
    virtual QPolygonF fitCurve(const QPolygonF &polygon) const = 0;

protected:
    QwtCurveFitter();

private:
    QwtCurveFitter(const QwtCurveFitter &);
    QwtCurveFitter &operator=(const QwtCurveFitter &);
};

/*!
  \brief A curve fitter using cubic splines
*/
class QWT_EXPORT QwtSplineCurveFitter : public QwtCurveFitter
{
public:
    enum FitMode { Auto, Spline, ParametricSpline };

    QwtSplineCurveFitter();
    virtual ~QwtSplineCurveFitter();

    void setFitMode(FitMode);
    FitMode fitMode() const;

    void setSpline(const QwtSpline &);
    const QwtSpline &spline() const;
    QwtSpline &spline();

    void setSplineSize(int size);
    int splineSize() const;

    virtual QPolygonF fitCurve(const QPolygonF &) const;

private:
    QPolygonF fitSpline(const QPolygonF &) const;
    QPolygonF fitParametric(const QPolygonF &) const;

    class PrivateData;
    PrivateData *d_data;
};

#endif

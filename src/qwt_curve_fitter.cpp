/* -*- mode: C++ ; c-file-style: "stroustrup" -*- *****************************
 * Qwt Widget Library
 * Copyright (C) 1997   Josef Wilgen
 * Copyright (C) 2002   Uwe Rathmann
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the Qwt License, Version 1.0
 *****************************************************************************/

#include "qwt_math.h"
#include "qwt_spline.h"
#include "qwt_curve_fitter.h"

//! Constructor
QwtCurveFitter::QwtCurveFitter() { }

//! Destructor
QwtCurveFitter::~QwtCurveFitter() { }

class QwtSplineCurveFitter::PrivateData
{
public:
    PrivateData() : fitMode(QwtSplineCurveFitter::Auto), splineSize(250) { }

    QwtSpline spline;
    QwtSplineCurveFitter::FitMode fitMode;
    int splineSize;
};

//! Constructor
QwtSplineCurveFitter::QwtSplineCurveFitter()
{
    d_data = new PrivateData;
}

//! Destructor
QwtSplineCurveFitter::~QwtSplineCurveFitter()
{
    delete d_data;
}

/*!
  Select the algorithm used for building the spline

  \param mode Mode representing a spline algorithm
  \sa fitMode()
*/
void QwtSplineCurveFitter::setFitMode(FitMode mode)
{
    d_data->fitMode = mode;
}

/*!
  \return Mode representing a spline algorithm
  \sa setFitMode()
*/
QwtSplineCurveFitter::FitMode QwtSplineCurveFitter::fitMode() const
{
    return d_data->fitMode;
}

void QwtSplineCurveFitter::setSpline(const QwtSpline &spline)
{
    d_data->spline = spline;
    d_data->spline.reset();
}

const QwtSpline &QwtSplineCurveFitter::spline() const
{
    return d_data->spline;
}

QwtSpline &QwtSplineCurveFitter::spline()
{
    return d_data->spline;
}

/*!
   Assign a spline size ( has to be at least 10 points )

   \param splineSize Spline size
   \sa splineSize()
*/
void QwtSplineCurveFitter::setSplineSize(int splineSize)
{
    d_data->splineSize = qwtMax(splineSize, 10);
}

/*!
  \return Spline size
  \sa setSplineSize()
*/
int QwtSplineCurveFitter::splineSize() const
{
    return d_data->splineSize;
}

/*!
  Find a curve which has the best fit to a series of data points

  \param points Series of data points
  \return Curve points
*/
QPolygonF QwtSplineCurveFitter::fitCurve(const QPolygonF &points) const
{
    const int size = (int)points.size();
    if (size <= 2)
        return points;

    FitMode fitMode = d_data->fitMode;
    if (fitMode == Auto) {
        fitMode = Spline;

        const QPointF *p = points.data();
        for (int i = 1; i < size; i++) {
            if (p[i].x() <= p[i - 1].x()) {
                fitMode = ParametricSpline;
                break;
            }
        };
    }

    if (fitMode == ParametricSpline)
        return fitParametric(points);
    else
        return fitSpline(points);
}

QPolygonF QwtSplineCurveFitter::fitSpline(const QPolygonF &points) const
{
    d_data->spline.setPoints(points);
    if (!d_data->spline.isValid())
        return points;

    QPolygonF fittedPoints(d_data->splineSize);

    const double x1 = points[0].x();
    const double x2 = points[int(points.size() - 1)].x();
    const double dx = x2 - x1;
    const double delta = dx / (d_data->splineSize - 1);

    for (int i = 0; i < d_data->splineSize; i++) {
        QPointF &p = fittedPoints[i];

        const double v = x1 + i * delta;
        const double sv = d_data->spline.value(v);

        p.setX(v);
        p.setY(sv);
    }
    d_data->spline.reset();

    return fittedPoints;
}

QPolygonF QwtSplineCurveFitter::fitParametric(const QPolygonF &points) const
{
    int i;
    const int size = points.size();

    QPolygonF fittedPoints(d_data->splineSize);
    QPolygonF splinePointsX(size);
    QPolygonF splinePointsY(size);

    const QPointF *p = points.data();
    QPointF *spX = splinePointsX.data();
    QPointF *spY = splinePointsY.data();

    double param = 0.0;
    for (i = 0; i < size; i++) {
        const double x = p[i].x();
        const double y = p[i].y();
        if (i > 0) {
            const double delta = sqrt(qwtSqr(x - spX[i - 1].y()) + qwtSqr(y - spY[i - 1].y()));
            param += qwtMax(delta, 1.0);
        }
        spX[i].setX(param);
        spX[i].setY(x);
        spY[i].setX(param);
        spY[i].setY(y);
    }

    d_data->spline.setPoints(splinePointsX);
    if (!d_data->spline.isValid())
        return points;

    const double deltaX = splinePointsX[size - 1].x() / (d_data->splineSize - 1);
    for (i = 0; i < d_data->splineSize; i++) {
        const double dtmp = i * deltaX;
        fittedPoints[i].setX(d_data->spline.value(dtmp));
    }

    d_data->spline.setPoints(splinePointsY);
    if (!d_data->spline.isValid())
        return points;

    const double deltaY = splinePointsY[size - 1].x() / (d_data->splineSize - 1);
    for (i = 0; i < d_data->splineSize; i++) {
        const double dtmp = i * deltaY;
        fittedPoints[i].setY(d_data->spline.value(dtmp));
    }

    return fittedPoints;
}

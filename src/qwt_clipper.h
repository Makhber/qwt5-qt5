/* -*- mode: C++ ; c-file-style: "stroustrup" -*- *****************************
 * Qwt Widget Library
 * Copyright (C) 1997   Josef Wilgen
 * Copyright (C) 2002   Uwe Rathmann
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the Qwt License, Version 1.0
 *****************************************************************************/

#ifndef QWT_CLIPPER_H
#define QWT_CLIPPER_H

#include "qwt_global.h"
#include "qwt_double_interval.h"

#include <QPolygon>
#include <QPolygonF>

class QRect;

/*!
  \brief Some clipping algos
*/

class QWT_EXPORT QwtClipper
{
public:
    static QPolygon clipPolygon(const QRect &, const QPolygon &);
    static QPolygonF clipPolygonF(const QRectF &, const QPolygonF &);

    static QVector<QwtDoubleInterval> clipCircle(const QRectF &, const QPointF &, double radius);
};

#endif

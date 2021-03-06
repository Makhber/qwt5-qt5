#include <qlayout.h>
#include <qwt_compass.h>
#include <qwt_compass_rose.h>
#include <qwt_dial_needle.h>
#include "compass_grid.h"

typedef QPalette Palette;

CompassGrid::CompassGrid(QWidget *parent) : QFrame(parent)
{
    QPalette p = palette();
    p.setColor(backgroundRole(), Qt::gray);
    setPalette(p);

    setAutoFillBackground(true);

    QGridLayout *layout = new QGridLayout(this);
    layout->setSpacing(5);
    layout->setMargin(0);

    int i;
    for (i = 0; i < 6; i++) {
        QwtCompass *compass = createCompass(i);
        layout->addWidget(compass, i / 3, i % 3);
    }

    for (i = 0; i < layout->columnCount(); i++)
        layout->setColumnStretch(i, 1);
}

QwtCompass *CompassGrid::createCompass(int pos)
{
    int c;

    Palette colorGroup;
    for (c = 0; c < Palette::NColorRoles; c++)
        colorGroup.setColor((Palette::ColorRole)c, QColor());

    colorGroup.setColor(Palette::Base, palette().color(backgroundRole()).lighter(120));
    colorGroup.setColor(Palette::WindowText, colorGroup.color(Palette::Base));

    QwtCompass *compass = new QwtCompass(this);
    compass->setLineWidth(4);
    compass->setFrameShadow(pos <= 2 ? QwtCompass::Sunken : QwtCompass::Raised);

    switch (pos) {
    case 0: {
        /*
          A compass with a rose and no needle. Scale and rose are
          rotating.
         */
        compass->setMode(QwtCompass::RotateScale);

        QwtSimpleCompassRose *rose = new QwtSimpleCompassRose(16, 2);
        rose->setWidth(0.15);

        compass->setRose(rose);
        break;
    }
    case 1: {
        /*
          A windrose, with a scale indicating the main directions only
         */
        QMap<double, QString> map;
        map.insert(0.0, "N");
        map.insert(90.0, "E");
        map.insert(180.0, "S");
        map.insert(270.0, "W");

        compass->setLabelMap(map);

        QwtSimpleCompassRose *rose = new QwtSimpleCompassRose(4, 1);
        compass->setRose(rose);

        compass->setNeedle(new QwtCompassWindArrow(QwtCompassWindArrow::Style2));
        compass->setValue(60.0);
        break;
    }
    case 2: {
        /*
          A compass with a rotating needle in darkBlue. Shows
          a ticks for each degree.
         */

        colorGroup.setColor(Palette::Base, Qt::darkBlue);
        colorGroup.setColor(Palette::WindowText, QColor(Qt::darkBlue).darker(120));
        colorGroup.setColor(Palette::Text, Qt::white);

        compass->setScaleOptions(QwtDial::ScaleTicks | QwtDial::ScaleLabel);
        compass->setScaleTicks(1, 1, 3);
        compass->setScale(36, 5, 0);

        compass->setNeedle(new QwtCompassMagnetNeedle(QwtCompassMagnetNeedle::ThinStyle));
        compass->setValue(220.0);

        break;
    }
    case 3: {
        /*
          A compass without a frame, showing numbers as tick labels.
          The origin is at 220.0
         */
        colorGroup.setColor(Palette::Base, palette().color(backgroundRole()));
        colorGroup.setColor(Palette::WindowText, Qt::blue);

        compass->setLineWidth(0);

        compass->setScaleOptions(QwtDial::ScaleBackbone | QwtDial::ScaleTicks
                                 | QwtDial::ScaleLabel);
        compass->setScaleTicks(0, 0, 3);

        QMap<double, QString> map;
        for (double d = 0.0; d < 360.0; d += 60.0) {
            QString label;
            label = QString::number(d, 'f', 0);
            map.insert(d, label);
        }
        compass->setLabelMap(map);
        compass->setScale(36, 5, 0);

        compass->setNeedle(new QwtDialSimpleNeedle(QwtDialSimpleNeedle::Ray, false, Qt::white));
        compass->setOrigin(220.0);
        compass->setValue(20.0);
        break;
    }
    case 4: {
        /*
         A compass showing another needle
         */
        compass->setScaleOptions(QwtDial::ScaleTicks | QwtDial::ScaleLabel);
        compass->setScaleTicks(0, 0, 3);

        compass->setNeedle(new QwtCompassMagnetNeedle(QwtCompassMagnetNeedle::TriangleStyle,
                                                      Qt::white, Qt::red));
        compass->setValue(220.0);
        break;
    }
    case 5: {
        /*
         A compass with a yellow on black ray
         */
        colorGroup.setColor(Palette::WindowText, Qt::black);

        compass->setNeedle(new QwtDialSimpleNeedle(QwtDialSimpleNeedle::Ray, false, Qt::yellow));
        compass->setValue(315.0);
        break;
    }
    }

    QPalette newPalette = compass->palette();
    for (c = 0; c < Palette::NColorRoles; c++) {
        if (colorGroup.color((Palette::ColorRole)c).isValid()) {
            for (int cg = 0; cg < QPalette::NColorGroups; cg++) {
                newPalette.setColor((QPalette::ColorGroup)cg, (Palette::ColorRole)c,
                                    colorGroup.color((Palette::ColorRole)c));
            }
        }
    }

    for (int i = 0; i < QPalette::NColorGroups; i++) {
        QPalette::ColorGroup cg = (QPalette::ColorGroup)i;

        const QColor light = newPalette.color(cg, Palette::Base).lighter(170);
        const QColor dark = newPalette.color(cg, Palette::Base).darker(170);
        const QColor mid = compass->frameShadow() == QwtDial::Raised
                ? newPalette.color(cg, Palette::Base).darker(110)
                : newPalette.color(cg, Palette::Base).lighter(110);

        newPalette.setColor(cg, Palette::Dark, dark);
        newPalette.setColor(cg, Palette::Mid, mid);
        newPalette.setColor(cg, Palette::Light, light);
    }
    compass->setPalette(newPalette);

    return compass;
}

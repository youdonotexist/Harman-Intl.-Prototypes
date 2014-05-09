#ifndef BEZIERCURVEHELPER_H
#define BEZIERCURVEHELPER_H

#include <QVector2D>

class BezierCurveHelper
{
public:
    BezierCurveHelper();
    double BezierPt(double t, double P0, double P1, double P2, double P3);
    QVector2D parametricPoint(QVector2D p1, QVector2D p2, QVector2D p3, double t);
};

#endif // BEZIERCURVEHELPER_H

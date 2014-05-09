#include "beziercurvehelper.h"

#include <math.h>

BezierCurveHelper::BezierCurveHelper()
{
}

double BezierCurveHelper::BezierPt(double t, double P0, double P1, double P2, double P3) {
  return
    powf(1-t, 3) * P0
    + 3 * powf(1-t, 2) * t * P1
    + 3 * (1-t) * powf(t, 2) * P2
    + powf(t, 3) * P3;
}

QVector2D BezierCurveHelper::parametricPoint(QVector2D p1, QVector2D p2, QVector2D p3, double t) {
    float x, y;

    // The Black Dot
    x = BezierPt(t, p1.x(), p2.x() - 5.0, p2.x() + 5.0, p3.x());
    y = BezierPt(t, p1.y(), p2.y(), p2.y(), p3.y());

    return QVector2D(x, y);
}

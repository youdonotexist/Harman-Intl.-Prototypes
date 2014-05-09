#ifndef PEQGRAPH_H
#define PEQGRAPH_H

#include <QDynamicPropertyChangeEvent>

#include "graph.h"
#include "CurveCalculator.h"

/**
 * @brief The PEQGraph class
 * @variable horizontalDivisions - Controls the granularity of the curve
 * @variable curveColor - Controls the rendered color of the curve
 */

class PEQGraph : public Graph
{
    Q_OBJECT

public:
    PEQGraph(QQuickItem *parent = 0);

    Q_PROPERTY (int horizontalDivisions READ horizontalDivisions WRITE setHorizontalDivisions NOTIFY horizontalDivisionsChanged)
    Q_PROPERTY (QColor curveColor READ curveColor WRITE setCurveColor NOTIFY curveColorChanged)

    Q_INVOKABLE QVector2D getKnobGraphPosition(int n);
    Q_INVOKABLE void setKnobGraphPosition(int n, double x, double y);
    Q_INVOKABLE int channelCount();
    Q_INVOKABLE void onKnobCountChanged();

    Q_INVOKABLE void requestUpdatedValues(int n);

    Q_INVOKABLE void onGainChanged(int n, float amount);
    Q_INVOKABLE void onQChanged(int n, float amount);
    Q_INVOKABLE void onFreqChanged(int n, float amount);

    QSGNode *updatePaintNode(QSGNode *, UpdatePaintNodeData * data);

    int horizontalDivisions() {return HorizontalDivisions;}
    void setHorizontalDivisions(int divs);

    QColor curveColor() {return m_curveColor;}
    void setCurveColor(QColor c) {
        m_curveColor = c;

        QColor dark = c.darker();
        dark.setAlphaF(0.7);
        m_darkerColor = dark;
    }

protected:
    void DrawPEQ(QSGNode * rootNode);

    void componentComplete();

    void _drawHump(QSGNode * rootNode, int n, QColor c1, QColor c2, bool showKnob);
    void _drawHumpOutline(QSGNode * parentNode, int n, QColor color);
    void _drawHumpSolid(QSGNode * parentNode, int n, QColor color);
    void _drawKnob(int n, QColor borderColor);

    bool eventFilter(QObject *obj, QEvent *event);

private:
    CurveCalculator curveCalc;
    QList<QObject*> knobs;

    int HorizontalDivisions;
    QColor m_curveColor;
    QColor m_darkerColor;
    int _hzTest;

    float ** computedData;
    int selectedIndex;

signals:
    void horizontalDivisionsChanged();
    void curveColorChanged();
};

#endif // PEQGRAPH_H

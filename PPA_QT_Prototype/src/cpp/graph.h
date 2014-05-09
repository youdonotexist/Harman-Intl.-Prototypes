#ifndef GRAPH_H
#define GRAPH_H

#include <qobject.h>
#include <qquickitem.h>


#include "axialinfo.h"

class Graph : public QQuickItem
{
    Q_OBJECT
    Q_PROPERTY (QColor color READ color WRITE setColor)
    Q_PROPERTY (QColor borderColor READ borderColor WRITE setBorderColor)
    Q_PROPERTY (QColor primaryLineColor READ primaryLineColor WRITE setPrimaryLineColor NOTIFY primaryLineColorChanged)
    Q_PROPERTY (QVector2D labelPadding READ labelPadding WRITE setLabelPadding NOTIFY labelPaddingChanged)

    Q_PROPERTY (bool simple READ simple WRITE setSimple NOTIFY simpleChanged)

public:
    Graph(QQuickItem *parent = 0);

    QColor color() {return m_color;}
    void setColor(QColor color) {m_color = color;}

    QColor primaryLineColor() {return m_primaryLineColor;}
    void setPrimaryLineColor(QColor color) {m_primaryLineColor = color;}

    QColor borderColor() {return m_borderColor;}
    void setBorderColor(QColor color) {m_borderColor = color;}

    QVector2D labelPadding() {return m_labelPadding;}
    void setLabelPadding(QVector2D padding) {m_labelPadding = padding;}

    bool simple() {return m_simple;}
    virtual void setSimple(bool simple) {
        m_simple = simple;
    }

    static const int ChannelCount = 8;

    /*
     * Overridden Methods
     */

    QSGNode *updatePaintNode(QSGNode *, UpdatePaintNodeData *);

    /*
     * Class Methods
     */
    Q_INVOKABLE float graphToNodeX(float graphX);
    Q_INVOKABLE float graphToNodeY(float graphY);
    Q_INVOKABLE float nodeToGraphX(float nodeX);
    Q_INVOKABLE float nodeToGraphY(float nodeY);

    QVector2D graphToNode(QVector2D graph);
    QVector2D nodeToGraph(QVector2D node);

    void DrawAxes(QSGNode * root);
    void DrawBackground(QSGNode * rootNode);
    void DrawXAxis(QSGNode * rootNode);
    void DrawYAxis(QSGNode * rootNode);

    void _drawLine(QSGNode * rootNode, QColor color, float startx, float starty,  float endx, float endy, float width);
    void _drawLabel(float x, float y, QString text, QHash<QString, QQuickItem*> &labelCache);

protected:
    AxialInfo xAxis;
    AxialInfo yAxis;

private:
    QColor m_color;
    QColor m_primaryLineColor;
    QColor m_borderColor;
    QVector2D m_labelPadding;

    bool m_simple;

    float xLabelOffset;
    float yLabelOffset;

    QHash<QString, QQuickItem*> yAxisLabels;
    QHash<QString, QQuickItem*> xAxisLabels;

    QQmlComponent * labelComponent;

signals:
    void primaryLineColorChanged();
    void horizontalDivisionsChanged();
    void labelPaddingChanged();
    void simpleChanged();

};

#endif // GRAPH_H

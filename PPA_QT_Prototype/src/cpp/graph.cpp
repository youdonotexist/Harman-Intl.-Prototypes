/**
 * Graph is a base class for anything that needs to render axial details
 * It handles drawing of vertial and horizontal lines, ticks, labels
 *
 */

#include "graph.h"

#include <stdio.h>
#include <math.h>

#include <QtQuick/qsgflatcolormaterial.h>
#include <QSGSimpleRectNode>
#include <QVariant>
#include <QQmlProperty>
#include <QDate>
#include <qsggeometry.h>
#include <QQmlEngine>
#include <QQmlContext>
#include <QDir>

#include "Constants.h"

Graph::Graph(QQuickItem *parent) : QQuickItem(parent)
{
    Q_INIT_RESOURCE(resources);

    setFlag(ItemHasContents, true);

    xLabelOffset = 0;
    yLabelOffset = 0;

    labelComponent = NULL;

    //HorizontalDivisions = 500;
}

QSGNode* Graph::updatePaintNode(QSGNode *oldNode, UpdatePaintNodeData *data)
{   
    QSGNode* rootNode = QQuickItem::updatePaintNode(oldNode, data);

    if (rootNode == NULL) {
        rootNode = new QSGNode();
        rootNode->setFlag(QSGNode::OwnedByParent, true);
    }
    else {
        rootNode->removeAllChildNodes();
    }

    DrawAxes(rootNode);

    return rootNode;
}

void Graph::DrawAxes(QSGNode * rootNode) {
    if (!this->m_simple) {
        //DrawBackground(rootNode);
        DrawXAxis(rootNode);
        DrawYAxis(rootNode);

    }
}

void Graph::DrawBackground(QSGNode * rootNode) {
    QSGSimpleRectNode * node = new QSGSimpleRectNode(boundingRect(), m_color);
    rootNode->appendChildNode(node);
}

void Graph::DrawXAxis(QSGNode * rootNode) {
    QRectF bounds = boundingRect();
    // X AXIS FEATURES
    for (QList<float>::iterator i = xAxis.major.begin(); i != xAxis.major.end(); i++) {
        float x = graphToNodeX(*i);
        float y = boundingRect().height() + xLabelOffset;

        _drawLine(rootNode, m_primaryLineColor, x, bounds.top(), x, bounds.bottom(), 2);
        _drawLabel(x, y, QString::number(*i), xAxisLabels);
    }

    for (QList<float>::iterator i = xAxis.minor.begin(); i != xAxis.minor.end(); i++) {
        float x = graphToNodeX(*i);
        float y = boundingRect().height() + xLabelOffset;

        _drawLine(rootNode, m_primaryLineColor.darker(),x, bounds.top(), x, bounds.bottom(), -1);
        _drawLabel(x, y, QString::number(*i), xAxisLabels);
    }

    for (QList<float>::iterator i = xAxis.subminor.begin(); i != xAxis.subminor.end(); i++) {
        float x = graphToNodeX(*i);
        float y = boundingRect().height() + xLabelOffset;

        _drawLine(rootNode, m_primaryLineColor.darker(), x, bounds.bottom(), x, bounds.top(), 0.5);
        _drawLabel(x, y, QString::number(*i), xAxisLabels);
    }

    // We can draw ticks, but the PPA prototype didn't have any
    /*for (QList<float>::iterator i = xAxis.tick.begin(); i != xAxis.tick.end(); i++) {
        float x = graphToNodeX(*i);

        _drawLine(rootNode, QColor(255, 255, 255, 255), x, bounds.bottom(), x, bounds.bottom()+5, 2);
    }*/
}

void Graph::DrawYAxis(QSGNode * rootNode) {
    QRectF bounds = boundingRect();
    // Y AXIS FEATURES
    for (QList<float>::iterator i = yAxis.major.begin(); i != yAxis.major.end(); i++) {
        float x = yLabelOffset;
        float y = graphToNodeY(*i);

        _drawLine(rootNode, m_primaryLineColor, bounds.left(), y, bounds.right(), y, 2);
        _drawLabel(x, y, QString::number(*i), yAxisLabels);
    }

    for (QList<float>::iterator i = yAxis.minor.begin(); i != yAxis.minor.end(); i++) {
        float x = yLabelOffset;
        float y = graphToNodeY(*i);

        _drawLine(rootNode, m_primaryLineColor.darker(), bounds.left(), y, bounds.right(), y, 1);
        _drawLabel(x, y, QString::number(*i), yAxisLabels);
    }

    for (QList<float>::iterator i = yAxis.subminor.begin(); i != yAxis.subminor.end(); i++) {
        float x = yLabelOffset;
        float y = graphToNodeY(*i);

        _drawLine(rootNode, m_primaryLineColor.darker(), bounds.left(), y, bounds.right(), y, 1);
        _drawLabel(x, y, QString::number(*i), yAxisLabels);
    }

    // We can draw ticks, but the PPA prototype didn't have any
    /*for (QList<float>::iterator i = yAxis.tick.begin(); i != yAxis.tick.end(); i++) {
        float y = graphToNodeY(*i);

        _drawLine(rootNode, QColor(255, 255, 255, 255), bounds.left(), y, bounds.left()-5, y, 2);
    }*/
}

float Graph::graphToNodeX(float graphX) {
    float norm;

    if (xAxis.mode == Linear) {
        norm = (graphX - xAxis.min) / (xAxis.max - xAxis.min);
    }
    else {
        float expRange = log10f(xAxis.max / xAxis.min);

        norm = log10f(graphX / xAxis.min) / expRange;
    }

    return norm * (boundingRect().width());
}

float Graph::graphToNodeY(float graphY) {
    float norm;

    if (yAxis.mode == Linear) {
        norm = (graphY - yAxis.min) / (yAxis.max - yAxis.min);
    }
    else {
        float expRange = log10f(yAxis.max / yAxis.min);

        norm = log10f(graphY / yAxis.min) / expRange;
    }

    return norm * (boundingRect().height());
}

float Graph::nodeToGraphX(float nodeX) {
    float norm = (nodeX) / (boundingRect().width());

    if (xAxis.mode == Linear) {
        return norm * (xAxis.max - xAxis.min) + xAxis.min;
    }
    else {
        float expRange = log10f(xAxis.max / xAxis.min);

        return powf(10.0f, norm * expRange) * xAxis.min;
    }
}

float Graph::nodeToGraphY(float nodeY) {
    float norm = (nodeY) / (boundingRect().height());

    if (yAxis.mode == Linear) {
        return norm * (yAxis.max - yAxis.min) + yAxis.min;
    }
    else {
        float expRange = log10f(yAxis.max / yAxis.min);

        return powf(10.0f, norm * expRange) * yAxis.min;
    }
}

QVector2D Graph::graphToNode(QVector2D graph) {
    QVector2D ret;
    ret.setX(graphToNodeX(graph.x()));
    ret.setY(graphToNodeY(graph.y()));
    return ret;
}

QVector2D Graph::nodeToGraph(QVector2D node) {
    QVector2D ret;
    ret.setX(nodeToGraphX(node.x()));
    ret.setY(nodeToGraphY(node.y()));
    return ret;
}

void Graph::_drawLine(QSGNode * rootNode, QColor color, float startx, float starty,  float endx, float endy, float width) {
    QSGSimpleRectNode * node = new QSGSimpleRectNode();
    node->setFlag(QSGNode::OwnedByParent);
    node->markDirty(QSGNode::DirtyForceUpdate);

    if (startx == endx) { //Vertical Line
        node->setRect(startx, endy, -width, starty - endy);
    }
    else { //Horizontal Line
        node->setRect(startx, endy, endx - startx, width);
    }

    node->setColor(color);
    rootNode->appendChildNode(node);
}

void Graph::_drawLabel(float x, float y, QString text, QHash<QString, QQuickItem*> &labelCache) {
    if(labelComponent == NULL) {
        QQmlEngine * engine = new QQmlEngine();
        labelComponent = new QQmlComponent(engine, QUrl("qrc:/src/qml/controls/common/_Text.qml", QUrl::StrictMode));

        if( labelComponent->status() != QQmlComponent::Ready)
        {
            qDebug() << ("Error:"+ labelComponent->errorString() );
            return; // or maybe throw
        }
    }

    if( labelComponent->status() != QQmlComponent::Ready)
    {
        return;
    }


    QQuickItem * label = labelCache[text];

    if (!label) { //TODO: Someday, scene graph might have a better text renderer
        label= qobject_cast<QQuickItem*>(labelComponent->create());
        label->setParentItem(this->parentItem()); //TODO: Who REALLY owns this item? The component? Or the Hash?

        label->setProperty("text", text);
        label->setProperty("width", "parent.width");
        label->children().first()->setProperty("color", m_primaryLineColor.darker());

        labelCache[text] = label;
    }

    if (label != NULL) {
        label->setProperty("x", x);
        label->setProperty("y", y);
    }
}


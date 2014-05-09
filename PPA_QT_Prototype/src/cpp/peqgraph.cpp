#include "peqgraph.h"

#include <math.h>
#include <qsgflatcolormaterial.h>
#include <qsgnode.h>

#include "Constants.h"
#include "dynamicpropertyhandler.h"

/**
 * PEQGraph is used to render equalizer graphs. It's composed of a number of bands represented by curves and a
 * number of knobs used to manipulate the values for each band.
 *
 * Knobs are created dynamically on touch and are connected to the knobmove in QML.
 *
 */

PEQGraph::PEQGraph(QQuickItem *parent) : Graph(parent)
{
    AxialInfo&x = xAxis;
    AxialInfo&y = yAxis;

    x.mode = Exponential;
    x.min = 20;
    x.max = 20000;

    y.mode = Linear;
    y.min = -20;
    y.max = 20;

    // PEQ Graph
    //Defines the axis details. Graham wants this stuff to be derived from the Storage layer
    if (!this->simple()) {
        x.minor.push_back(20);
        x.minor.push_back(40);
        x.minor.push_back(80);
        x.minor.push_back(200);
        x.minor.push_back(500);
        x.minor.push_back(1000);
        x.minor.push_back(2000);
        x.minor.push_back(5000);
        x.minor.push_back(10000);
        x.minor.push_back(20000);
        x.tick.push_back(20);
        x.tick.push_back(40);
        x.tick.push_back(80);
        x.tick.push_back(200);
        x.tick.push_back(500);
        x.tick.push_back(1000);
        x.tick.push_back(2000);
        x.tick.push_back(5000);
        x.tick.push_back(10000);
        x.tick.push_back(20000);

        y.minor.push_back(-20);
        y.minor.push_back(-12);
        y.minor.push_back(-6);
        y.major.push_back(0);
        y.minor.push_back(6);
        y.minor.push_back(12);
        y.minor.push_back(20);
        y.tick.push_back(-20);
        y.tick.push_back(-12);
        y.tick.push_back(-6);
        y.tick.push_back(0);
        y.tick.push_back(6);
        y.tick.push_back(12);
        y.tick.push_back(20);
    }

    curveCalc.ChannelCount = ChannelCount;
    curveCalc.isAFS = false;

    HorizontalDivisions = 0;
    selectedIndex = 0;
    computedData = NULL;

    //Initialize curve calc values
    for (int i = 0; i < ChannelCount; i++) {
        curveCalc.settings[i].gainDB = 0;//sign * 20.0f * (i+1) / ChannelCount;
        curveCalc.settings[i].freq = 20000;
        curveCalc.settings[i].Q = 1;
        curveCalc.settings[i].filterType = 0;
    }
}

void PEQGraph::setHorizontalDivisions(int divs) {
    if (divs != HorizontalDivisions) {
        HorizontalDivisions = divs;
        if (computedData != NULL)
            delete computedData;
        computedData = new float*[HorizontalDivisions+1];
        for(int i = 0; i < HorizontalDivisions+1; ++i) {
            computedData[i] = new float[ChannelCount + 2];
        }
    }
}

void PEQGraph::componentComplete() {
    Graph::componentComplete();
    for (int i = 0; i < ChannelCount; i++) {
       // emit ValueChangeCallback(i, "gain", curveCalc.settings[i].gainDB);
       // emit ValueChangeCallback(i, "q", curveCalc.settings[i].Q);
    }
}

//TODO: This was a cool idea. Investigate if it's possible set a property on the QML side dynamically
bool PEQGraph::eventFilter(QObject *, QEvent *event)
{
    if (event->type() == QEvent::DynamicPropertyChange) {
        //QDynamicPropertyChangeEvent * dynamicEvent = static_cast<QDynamicPropertyChangeEvent*>(event);

        return true;
    }

    return false;
}

QSGNode *PEQGraph::updatePaintNode(QSGNode *oldNode, UpdatePaintNodeData * data)
{
    QSGNode * rootNode = Graph::updatePaintNode(oldNode, data);

    if (HorizontalDivisions != 0) {
        DrawPEQ(rootNode);
    }

    return rootNode;
}

QVector2D PEQGraph::getKnobGraphPosition( int n ) {
    return graphToNode(QVector2D(curveCalc.settings[n].freq, curveCalc.settings[n].gainDB));
}

void PEQGraph::setKnobGraphPosition(int n, double x, double y) {
    QVector2D pos = nodeToGraph(QVector2D(x, y));

    curveCalc.settings[n].freq = pos.x();
    curveCalc.settings[n].gainDB = pos.y();

    selectedIndex = n;

    update();
}

int PEQGraph::channelCount() {
    return ChannelCount;
}

void PEQGraph::onKnobCountChanged() {
    knobs = this->findChildren<QObject*>("knob");
    this->update();
}

void PEQGraph::requestUpdatedValues(int n) {

}

void PEQGraph::onGainChanged(int n, float gain) {
    curveCalc.settings[n].gainDB = gain;
    this->setProperty(QString("%1gain").arg(n).toLocal8Bit().data(), gain);
    this->update();

}

void PEQGraph::onQChanged(int n, float amount) {
    curveCalc.settings[n].Q = amount;
    this->setProperty(QString("%1q").arg(n).toLocal8Bit().data(), amount);
    this->update();

}

void PEQGraph::onFreqChanged(int n, float amount) {
    curveCalc.settings[n].freq = amount;
    this->setProperty(QString("%1freq").arg(n).toLocal8Bit().data(), amount);
    this->update();

}

void PEQGraph::DrawPEQ(QSGNode * rootNode) {

    for (int i = 0; i < ChannelCount; i++) {
        curveCalc.computeCoefficients(i);
    }

    // compute all graph points
    for (int i = 0; i < HorizontalDivisions+1; i++) {
        float v = curveCalc.computeGain(20.0f * pow(10.0f, 3.0f * ((float)i / (float)HorizontalDivisions)));
        if (v < -1) v = -1;
        else if (v > 1) v = 1;
        computedData[i][ChannelCount] = 20.0f * v;
        for (int n = 0; n < ChannelCount; n++) {
            computedData[i][n] = 20.0f * curveCalc.sectionResponses[n];
        }
    }

    for (int i = 0; i < HorizontalDivisions; i++) {
        double accum = 0.0;
        for (int n = 0; n < knobs.length(); n++) {
            accum += computedData[i][n];
        }

        computedData[i][ChannelCount+1] = accum;// / (double) knobs.length();
    }

    for (int i = 0; i < knobs.length(); i++) {
        if (i == selectedIndex) continue;
        _drawHump(rootNode, i, m_darkerColor, m_curveColor, true);
    }

    _drawHump(rootNode, selectedIndex, m_darkerColor, m_curveColor, true);
    _drawHump(rootNode, ChannelCount+1, m_darkerColor, m_curveColor, false);
}

void PEQGraph::_drawHump(QSGNode *rootNode, int n, QColor color1, QColor color2, bool showKnob) {
    QSGNode * parent = new QSGNode;
    if (showKnob) _drawHumpSolid(parent, n, color1);
    _drawHumpOutline(parent, n, color2);
    if (showKnob) _drawKnob(n, color2);

    rootNode->appendChildNode(parent);
}

void PEQGraph::_drawKnob(int n, QColor borderColor) {
    QVector2D mid = graphToNode(QVector2D(curveCalc.settings[n].freq, curveCalc.settings[n].gainDB));

    if (n < knobs.length()) {
         QObject * obj = knobs[n];
         if (obj) {
             obj->setProperty("x", mid.x());
             obj->setProperty("y", mid.y());
             obj->setProperty("borderColor", borderColor);
         }
    }
}

void PEQGraph::_drawHumpOutline(QSGNode * parentNode, int n, QColor color) {
    //Solid Outline
    QSGGeometryNode * lineNode = new QSGGeometryNode();
    QSGGeometry * lineGeom = new QSGGeometry(QSGGeometry::defaultAttributes_Point2D(), (HorizontalDivisions));

    QSGFlatColorMaterial *material2 = new QSGFlatColorMaterial;

    material2->setColor(color);
    material2->setFlag(QSGMaterial::Blending);

    lineNode->setMaterial(material2);
    lineNode->setFlag(QSGNode::OwnsGeometry);
    lineNode->setFlag(QSGNode::OwnedByParent);
    lineNode->setFlag(QSGNode::OwnsMaterial);

    lineGeom->setDrawingMode(GL_LINE_STRIP);
    glLineWidth(2.0f);

    QSGGeometry::Point2D* point2 = lineGeom->vertexDataAsPoint2D();

    float left = graphToNodeX(xAxis.min);
    float width = graphToNodeX(xAxis.max) - graphToNodeX(xAxis.min);
    float stride = width / HorizontalDivisions;

    for (int i = 0; i < HorizontalDivisions; i++) {
        float x1 = left + (i)*stride;
        float y1 = graphToNodeY(computedData[i][n]);

        point2->set(x1, y1);
        point2++;
    }

    lineNode->setGeometry(lineGeom);
    parentNode->appendChildNode(lineNode);
}

void PEQGraph::_drawHumpSolid(QSGNode * parentNode, int n, QColor color) {
    //Transparent shaded part//
    QSGGeometryNode * solidNode = new QSGGeometryNode();
    QSGGeometry * solidGeom = new QSGGeometry(QSGGeometry::defaultAttributes_Point2D(), 2 * (HorizontalDivisions + 1));
    QSGFlatColorMaterial *material = new QSGFlatColorMaterial;

    material->setColor(color);
    material->setFlag(QSGMaterial::Blending);

    solidNode->setMaterial(material);
    solidNode->setFlag(QSGNode::OwnsGeometry);
    solidNode->setFlag(QSGNode::OwnedByParent);
    solidNode->setFlag(QSGNode::OwnsMaterial);

    solidGeom->setDrawingMode(GL_TRIANGLE_STRIP);

    QSGGeometry::Point2D* point = solidGeom->vertexDataAsPoint2D();

    float left = graphToNodeX(xAxis.min);
    float mid = graphToNodeY(0);
    float width = graphToNodeX(xAxis.max) - graphToNodeX(xAxis.min);
    float stride = width / HorizontalDivisions;

    for (int i = 0; i < HorizontalDivisions + 1; i++) {
        point->set(left + i*stride, mid);
        point++;
        point->set(left + i*stride, graphToNodeY(computedData[i][n]));
        point++;
    }

    solidNode->setGeometry(solidGeom);
    parentNode->appendChildNode(solidNode);
}

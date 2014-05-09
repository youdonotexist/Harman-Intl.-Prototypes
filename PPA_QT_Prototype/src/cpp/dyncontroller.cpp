#include "dyncontroller.h"

#include "HIMath.h"

#include <qsgnode.h>
#include <qqmlengine.h>
#include <qqmlcomponent.h>
#include <qsgflatcolormaterial.h>
#include <math.h>
#include <QMutableListIterator>

/**
 * The DynamicController is composed of a few lines and a few knobs,
 * depending on the type (Compressor, Gate, Gate+Compressor)
 *
 * Knobs are createded in C++ (which differs from the peqgraph.c) and
 * connected to the graph through the knobmove signal.
 *
 * The _tempModel object is just a placeholder for a Storage layer implementation
 *
 */

DynamicController::DynamicController(QQuickItem *parent) : Graph(parent)
{
    xAxis.mode = Linear;
    xAxis.min = -90;
    xAxis.max = 30;
    xAxis.minor.push_back(-30);

    yAxis.mode = Linear;
    yAxis.min = -90;
    yAxis.max = 30;
    yAxis.minor.push_back(-30);

    knobComponent = NULL;

    _tempModel["compx"] = -30;
    _tempModel["compy"] = -30;

    _tempModel["ratio"] = 1;

    _tempModel["gatex"] = -30;
    _tempModel["gatey"] = -30;
}

QSGNode *DynamicController::updatePaintNode(QSGNode * oldNode, UpdatePaintNodeData *data) {
    QSGNode * rootNode = Graph::updatePaintNode(oldNode, data);

    if (m_type == Compressor) {
        drawCompressor(rootNode);
    }
    else if (m_type == Gate) {
        drawGate(rootNode);
    }
    else if (m_type == GateCompressor) {
        drawGateCompressor(rootNode);
    }

    return rootNode;
}

void DynamicController::initKnobs() {
    QMutableListIterator<QObject*> i(knobs);
    while (i.hasNext()) {
        QObject * o = i.next();
        i.remove();
        delete o;
    }

    if (!this->simple()) {
        if (m_type == Compressor) { //Init two knobs
            QQuickItem * knob1 = createKnob();

            if (!knob1) {
                return;
            }

            knob1->setParentItem(this);
            knob1->setProperty("width", "parent.width * 0.1");
            knob1->setProperty("height", "parent.height * 0.1");
            knob1->setProperty("x", graphToNodeX(_tempModel["compx"]));
            knob1->setProperty("y", graphToNodeY(_tempModel["compy"]));
            knob1->setProperty("index", 0);

            QQuickItem * knob2 = createKnob();
            knob2->setParentItem(this);
            knob2->setProperty("width", "parent.width * 0.1");
            knob2->setProperty("height", "parent.height * 0.1");
            knob2->setProperty("x", graphToNodeX(yAxis.max));
            knob2->setProperty("y", graphToNodeY(_tempModel["compy"] * (1 - _tempModel["ratio"])));
            knob2->setProperty("index", 1);

            knobs.append(knob1);
            knobs.append(knob2);
        }
        else if (m_type == Gate) {
            QQuickItem * knob1 = createKnob();
            if (!knob1) return;
            knob1->setParentItem(this);
            knob1->setProperty("width", "parent.width * 0.1");
            knob1->setProperty("height", "parent.height * 0.1");
            knob1->setProperty("x", graphToNodeX(_tempModel["gatex"]));
            knob1->setProperty("y", graphToNodeY(_tempModel["gatey"]));
            knob1->setProperty("index", 0);

            knobs.append(knob1);
        }
        else if (m_type == GateCompressor) {
            QQuickItem * knob1 = createKnob();
            if (!knob1) return;
            knob1->setParentItem(this);
            knob1->setProperty("width", "parent.width * 0.1");
            knob1->setProperty("height", "parent.height * 0.1");
            knob1->setProperty("x", graphToNodeX(_tempModel["gatex"]));
            knob1->setProperty("y", graphToNodeY(_tempModel["gatey"]));
            knob1->setProperty("index", 0);

            QQuickItem * knob2 = createKnob();
            knob2->setParentItem(this);
            knob2->setProperty("width", "parent.width * 0.1");
            knob2->setProperty("height", "parent.height * 0.1");
            knob2->setProperty("x", graphToNodeX(_tempModel["compx"]));
            knob2->setProperty("y", graphToNodeY(_tempModel["compy"]));
            knob2->setProperty("index", 1);

            QQuickItem * knob3 = createKnob();
            knob3->setParentItem(this);
            knob3->setProperty("width", "parent.width * 0.1");
            knob3->setProperty("height", "parent.height * 0.1");
            knob3->setProperty("x", graphToNodeX(yAxis.max));
            knob3->setProperty("y", graphToNodeY(_tempModel["compy"] * (1 - _tempModel["ratio"])));
            knob3->setProperty("index", 2);

            knobs.append(knob1);
            knobs.append(knob2);
            knobs.append(knob3);
        }
    }
}

void DynamicController::drawCompressor(QSGNode * rootNode) {
    //Update knobs
    if (!this->simple()) {
        QObject * threshKnob = knobs[0];
        QObject * ratKnob = knobs[1];

        threshKnob->setProperty("x", graphToNodeX(_tempModel["compx"]));
        threshKnob->setProperty("y", graphToNodeY(_tempModel["compy"]));

        ratKnob->setProperty("x", graphToNodeX(yAxis.max));
        ratKnob->setProperty("y", graphToNodeY(_tempModel["compy"]) * (1 - _tempModel["ratio"]));
    }

    QSGGeometryNode * lineNode = new QSGGeometryNode();
    QSGGeometry * lineGeom = new QSGGeometry(QSGGeometry::defaultAttributes_Point2D(), (3));

    QSGFlatColorMaterial *material = new QSGFlatColorMaterial;

    material->setColor(this->curveColor());
    material->setFlag(QSGMaterial::Blending);

    lineNode->setMaterial(material);
    lineNode->setFlag(QSGNode::OwnsGeometry);
    lineNode->setFlag(QSGNode::OwnedByParent);
    lineNode->setFlag(QSGNode::OwnsMaterial);

    lineGeom->setDrawingMode(GL_LINE_STRIP);
    glLineWidth(2.0f);

    QSGGeometry::Point2D* points = lineGeom->vertexDataAsPoint2D();

    QRectF bounds = boundingRect();

    points->set(bounds.left(), bounds.bottom()); points++;

    for (int i = 0; i < 2; i++) {
        QQuickItem * q = qobject_cast<QQuickItem*>(knobs[i]);
        points->set(q->x(), q->y());
        points++;
    }

    lineNode->setGeometry(lineGeom);
    rootNode->appendChildNode(lineNode);
}

void DynamicController::drawGate(QSGNode * rootNode) {
    //Update knobs
    if (!this->simple()) {
        QObject * gateKnob = knobs[0];

        gateKnob->setProperty("x", graphToNodeX(_tempModel["gatex"]));
        gateKnob->setProperty("y", graphToNodeY(_tempModel["gatey"]));
    }

    QSGGeometryNode * lineNode = new QSGGeometryNode();
    QSGGeometry * lineGeom = new QSGGeometry(QSGGeometry::defaultAttributes_Point2D(), (3));

    QSGFlatColorMaterial *material = new QSGFlatColorMaterial;

    material->setColor(this->curveColor());
    material->setFlag(QSGMaterial::Blending);

    lineNode->setMaterial(material);
    lineNode->setFlag(QSGNode::OwnsGeometry);
    lineNode->setFlag(QSGNode::OwnedByParent);
    lineNode->setFlag(QSGNode::OwnsMaterial);

    lineGeom->setDrawingMode(GL_LINE_STRIP);
    glLineWidth(2.0f);

    QSGGeometry::Point2D* points = lineGeom->vertexDataAsPoint2D();

    QRectF bounds = boundingRect();

    points->set(graphToNodeX(_tempModel["gatex"]), bounds.bottom()); points++;
    points->set(graphToNodeX(_tempModel["gatex"]), graphToNodeX(_tempModel["gatey"])); points++;
    points->set(bounds.right(), bounds.top()); points++;

    lineNode->setGeometry(lineGeom);
    rootNode->appendChildNode(lineNode);
}

void DynamicController::drawGateCompressor(QSGNode * rootNode) {

    if (knobs.count() == 0) {
        return;
    }
    //Update knobs
    if (!this->simple()) {
        QObject * gateKnob = knobs[0];
        QObject * compThreshKnob = knobs[1];
        QObject * compRatioKnob = knobs[2];

        gateKnob->setProperty("x", graphToNodeX(_tempModel["gatex"]));
        gateKnob->setProperty("y", graphToNodeY(_tempModel["gatey"]));

        compThreshKnob->setProperty("x", graphToNodeX(_tempModel["compx"]));
        compThreshKnob->setProperty("y", graphToNodeY(_tempModel["compy"]));

        compRatioKnob->setProperty("x", graphToNodeX(xAxis.max));
        compRatioKnob->setProperty("y", graphToNodeY(_tempModel["compy"]) * (1 - _tempModel["ratio"]));
    }

    QSGGeometryNode * lineNode = new QSGGeometryNode();
    QSGGeometry * lineGeom = new QSGGeometry(QSGGeometry::defaultAttributes_Point2D(), (4));

    QSGFlatColorMaterial *material = new QSGFlatColorMaterial;

    material->setColor(this->curveColor());
    material->setFlag(QSGMaterial::Blending);

    lineNode->setMaterial(material);
    lineNode->setFlag(QSGNode::OwnsGeometry);
    lineNode->setFlag(QSGNode::OwnedByParent);
    lineNode->setFlag(QSGNode::OwnsMaterial);

    lineGeom->setDrawingMode(GL_LINE_STRIP);
    glLineWidth(2.0f);

    QSGGeometry::Point2D* points = lineGeom->vertexDataAsPoint2D();

    QRectF bounds = boundingRect();

    points->set(graphToNodeX(_tempModel["gatex"]), bounds.bottom()); points++;
    points->set(graphToNodeX(_tempModel["gatex"]), graphToNodeY(_tempModel["gatey"])); points++;
    points->set(graphToNodeX(_tempModel["compx"]), graphToNodeY(_tempModel["compy"])); points++;
    points->set(graphToNodeX(xAxis.max), graphToNodeY(_tempModel["compy"]) * (1 - _tempModel["ratio"])); points++;

    lineNode->setGeometry(lineGeom);
    rootNode->appendChildNode(lineNode);
}

void DynamicController::setKnobGraphPosition(int n, double x, double y) {
    float outx = 0, outy = 0;
    if (m_type == Compressor) {
        if (n == 0) {
            //Constrain values to the line between botleft and topright
            //y = mx + b,
            //m = dy / dx = (bounds.top - bounds.bottom) / (bounds.right - bounds.left)
            //b = bounds.bottom
            QRectF bounds = boundingRect();
            float m = (bounds.top() - bounds.bottom()) / (bounds.right() - bounds.left());
            outx = x;
            outy = (m * outx) + bounds.bottom();

            _tempModel["compx"] = nodeToGraphX(HI_FMAX(HI_FMIN(outx, bounds.right()), bounds.left()));
            _tempModel["compy"] = nodeToGraphY(HI_FMAX(HI_FMIN(outy, bounds.bottom()), bounds.top()));
        }
        else {
            QObject * thresholdKnob = knobs[0];

            float nh = (thresholdKnob->property("y").toFloat());
            float ratio = HI_FMIN(HI_FMAX(0, 1 - (y / nh)), 1);
            _tempModel["ratio"] = ratio;
        }
    }
    else if (m_type == Gate) {
        if (n == 0) {
            QRectF bounds = boundingRect();
            float m = (bounds.top() - bounds.bottom()) / (bounds.right() - bounds.left());
            outx = x;
            outy = (m * outx) + bounds.bottom();

            _tempModel["gatex"] = nodeToGraphX(HI_FMAX(HI_FMIN(outx, bounds.right()), bounds.left()));
            _tempModel["gatey"] = nodeToGraphY(HI_FMAX(HI_FMIN(outy, bounds.bottom()), bounds.top()));
        }
    }
    else if (m_type == GateCompressor) {
        if (n == 0) {
            QRectF bounds = boundingRect();
            float m = (bounds.top() - bounds.bottom()) / (bounds.right() - bounds.left());
            outx = HI_FMIN(graphToNodeX(_tempModel["compx"]), x);
            outy = (m * outx) + bounds.bottom();

            _tempModel["gatex"] = nodeToGraphX(HI_FMAX(HI_FMIN(outx, bounds.right()), bounds.left()));
            _tempModel["gatey"] = nodeToGraphY(HI_FMAX(HI_FMIN(outy, bounds.bottom()), bounds.top()));
        }
        else if (n == 1) {
            //Constrain values to the line between botleft and topright
            //y = mx + b,
            //m = dy / dx = (bounds.top - bounds.bottom) / (bounds.right - bounds.left)
            //b = bounds.bottom
            QRectF bounds = boundingRect();
            float m = (bounds.top() - bounds.bottom()) / (bounds.right() - bounds.left());
            outx = HI_FMAX(graphToNodeX(_tempModel["gatex"]), x);
            outy = (m * outx) + bounds.bottom();

            _tempModel["compx"] = nodeToGraphX(HI_FMAX(HI_FMIN(outx, bounds.right()), bounds.left()));
            _tempModel["compy"] = nodeToGraphY(HI_FMAX(HI_FMIN(outy, bounds.bottom()), bounds.top()));
        }
        else {
            QObject * thresholdKnob = knobs[1];

            float nh = (thresholdKnob->property("y").toFloat());
            float ratio = HI_FMIN(HI_FMAX(0, 1 - (y / nh)), 1);
            _tempModel["ratio"] = ratio;
        }
    }

    update();
}

//Connect between knob and dyncontroller is made here. When the knob moves, it sends an index property with the new x,y position
QQuickItem * DynamicController::createKnob() {
    if (knobComponent == NULL) {
        QQmlEngine * engine = new QQmlEngine();

        knobComponent = new QQmlComponent(engine,QUrl("qrc:/src/qml/controls/common/Knob.qml"));
        if( knobComponent->status() != QQmlComponent::Ready)
        {
            qDebug() << ("Error:"+ knobComponent->errorString() );
        }
    }

    QQuickItem * obj = qobject_cast<QQuickItem*>(knobComponent->create());

    QObject::connect(obj, SIGNAL(knobmove(int, double, double)), this, SLOT(setKnobGraphPosition(int, double, double)));

    return obj;
}

DynamicController::~DynamicController() {
    delete knobComponent;
}

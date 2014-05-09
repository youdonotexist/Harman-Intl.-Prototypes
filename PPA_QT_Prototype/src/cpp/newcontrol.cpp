#include "newcontrol.h"

#include <QSGGeometryNode>
#include <math.h>
#include <QSGFlatColorMaterial>

NewControl::NewControl(QQuickItem *parent) :
    QQuickItem(parent)
{
    //This flag is important. It tells the scene graph "I have stuff inside me that needs to be rendered at least once!"
    setFlag(ItemHasContents, true);
}

void NewControl::updateRadius(float x, float y) {
    float cx = boundingRect().center().x();
    float cy = boundingRect().center().y();

    float dx = (cx - x);
    float dy = (cy - y);

    m_radius = (sqrtf((dx*dx) + (dy*dy)));
    emit radChanged(m_radius);
    update();
}

QSGNode * NewControl::updatePaintNode(QSGNode *oldNode, UpdatePaintNodeData * data) {
    QSGNode* rootNode = QQuickItem::updatePaintNode(oldNode, data);

    if (rootNode == NULL) {
        rootNode = new QSGNode();
        rootNode->setFlag(QSGNode::OwnedByParent, true);
    }
    else {
        rootNode->removeAllChildNodes();
    }

    QSGGeometryNode * lineNode = new QSGGeometryNode();
    QSGGeometry * lineGeom = new QSGGeometry(QSGGeometry::defaultAttributes_Point2D(), 102);

    QSGFlatColorMaterial *material2 = new QSGFlatColorMaterial;

    material2->setColor(QColor(255, 128, 128, 255));
    material2->setFlag(QSGMaterial::Blending);

    lineNode->setMaterial(material2);
    lineNode->setFlag(QSGNode::OwnsGeometry);
    lineNode->setFlag(QSGNode::OwnedByParent);
    lineNode->setFlag(QSGNode::OwnsMaterial);

    lineGeom->setDrawingMode(GL_TRIANGLE_FAN);
    glLineWidth(2.0f);

    QSGGeometry::Point2D* point = lineGeom->vertexDataAsPoint2D();
    float cx = boundingRect().center().x();
    float cy = boundingRect().center().y();
    float radius = boundingRect().width() * 0.5f;

    point->set(cx, cy);
    point++;

    for (int i = 0; i < 101; i++) {
        float ratio = (((float)i) / 100.0f);

        float x = cx + (cosf(ratio * (2 * M_PI)) * m_radius);
        float y =  cy + (sinf(ratio * (2 * M_PI)) * m_radius);
        point->set(x, y);
        point++;
    }

    lineNode->setGeometry(lineGeom);
    rootNode->appendChildNode(lineNode);
    return rootNode;
}

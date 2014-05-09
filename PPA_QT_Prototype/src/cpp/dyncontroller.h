#ifndef DYNCONTROLLER_H
#define DYNCONTROLLER_H

#include "graph.h"
#include <qmap.h>

class DynamicController : public Graph
{
    Q_OBJECT
    Q_ENUMS(Type)
    Q_PROPERTY(Type type READ type WRITE setType NOTIFY typeChanged)
    Q_PROPERTY (QColor curveColor READ curveColor WRITE setCurveColor NOTIFY curveColorChanged)

public:
    DynamicController(QQuickItem *parent = 0);
    ~DynamicController();

    Q_INVOKABLE void setKnobGraphPosition(int n, double x, double y);

    QColor curveColor() {return m_curveColor;}
    void setCurveColor(QColor c) {
        m_curveColor = c;

        QColor dark = c.darker();
        dark.setAlphaF(0.7);
        m_darkerColor = dark;
    }

    enum Type {
        Compressor, //One controls point, ratio
        Expander,
        Gate, //One control point
        Limiter,
        GateCompressor //Two Control Points, ratio
    };
    Type type() const { return m_type;}
    void setType(Type t) {
        m_type = t;
        this->initKnobs();
    }

    void setSimple(bool s) {
        Graph::setSimple(s);
        this->initKnobs();
    }

    /*
     * Overridden Methods
     */

    QSGNode *updatePaintNode(QSGNode * oldNode, UpdatePaintNodeData * data);

signals:
    void typeChanged();
    void curveColorChanged();

private:
    void drawCompressor(QSGNode * rootNode);
    void drawGate(QSGNode * rootNode);
    void drawGateCompressor(QSGNode * rootNode);
    void initKnobs();
    QQuickItem * createKnob();

    Type m_type;
    QList<QObject*> knobs;
    QQmlComponent * knobComponent;

    QColor m_curveColor;
    QColor m_darkerColor;

    QMap <QString, float> _tempModel;
};

#endif // DYNCONTROLLER_H

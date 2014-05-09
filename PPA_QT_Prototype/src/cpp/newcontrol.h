#ifndef NEWCONTROL_H
#define NEWCONTROL_H

#include <QQuickItem>

class NewControl : public QQuickItem
{
    Q_OBJECT
public:
    explicit NewControl(QQuickItem *parent = 0);

    //The singal Just Worked when I removed the notify here. Needs some investigation as to why
    //the notify overwrites the signal
    Q_PROPERTY (float radius READ radius WRITE setRadius)

    Q_INVOKABLE void updateRadius(float x, float y);

    QSGNode *updatePaintNode(QSGNode *oldNode, UpdatePaintNodeData * data);

    float radius() {return m_radius;}
    void setRadius(float rad) {m_radius = rad;}

signals:
    void radChanged(float);

public slots:

private:
    float m_radius;

};

#endif // NEWCONTROL_H

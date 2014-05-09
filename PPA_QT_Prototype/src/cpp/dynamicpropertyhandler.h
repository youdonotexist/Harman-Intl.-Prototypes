#ifndef DYNAMICPROPERTYHANDLER_H
#define DYNAMICPROPERTYHANDLER_H

#include <QObject>

class DynamicPropertyHandler : public QObject
{
    Q_OBJECT
public:
    explicit DynamicPropertyHandler(QObject *parent = 0);

protected:
    bool eventFilter(QObject *obj, QEvent *event);

signals:

public slots:

};

#endif // DYNAMICPROPERTYHANDLER_H

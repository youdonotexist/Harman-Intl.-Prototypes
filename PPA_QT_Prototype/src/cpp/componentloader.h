#ifndef COMPONENTLOADER_H
#define COMPONENTLOADER_H

#include <qmap.h>
#include <qqmlcomponent.h>

class ComponentLoader
{
public:
    ComponentLoader();

    QObject * createObject(QString path);

private:
    QMap<QString, QQmlComponent*> map;
};

#endif // COMPONENTLOADER_H

#include "componentloader.h"

#include <qqmlengine.h>

ComponentLoader::ComponentLoader()
{

}

QObject * ComponentLoader::createObject(QString path) {
    QQmlComponent * component = map[path];
    if (component == NULL) {
        QQmlEngine * engine = new QQmlEngine;
        component = new QQmlComponent(engine, path);

        if (component->status() != QQmlComponent::Error) {
            map[path] = component;
            return component->create();
        }
    }

    return NULL;
}

#include "dynamicpropertyhandler.h"
#include <QDebug>
#include <QEvent>

DynamicPropertyHandler::DynamicPropertyHandler(QObject *parent) :
    QObject(parent)
{   

}

bool DynamicPropertyHandler::eventFilter(QObject *, QEvent *event)
{
    if (event->type() == QEvent::DynamicPropertyChange) {
        //QDynamicPropertyChangeEvent * dynamicEvent = static_cast<QDynamicPropertyChangeEvent*>(event);

        return true;
    }

    return false;
}

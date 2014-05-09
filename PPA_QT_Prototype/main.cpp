#include <QtGui/QGuiApplication>
#include "qtquick2applicationviewer.h"
#include <qdebug.h>
#include <math.h>

#include "src/cpp/graph.h"
#include "src/cpp/peqgraph.h"
#include "src/cpp/dyncontroller.h"
#include "src/cpp/newcontrol.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    // set the value of the environment variable QML_IMPORT_TRACE as 1
    //QByteArray data = "1";
    //qputenv("QML_IMPORT_TRACE", data);

    qmlRegisterType<Graph>("HIControls", 1, 0, "Graph");
    qmlRegisterType<PEQGraph>("HIControls", 1, 0, "PEQGraph");
    qmlRegisterType<DynamicController>("HIControls", 1, 0, "DynamicController");
    qmlRegisterType<NewControl>("HIControls", 1, 0, "NewControl");

    QtQuick2ApplicationViewer viewer;
    viewer.setMainQmlFile(QStringLiteral("qml/qml/main.qml"));
    viewer.showExpanded();

    return app.exec();
}

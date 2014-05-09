# Add more folders to ship with the application, here
folder_01.source = src/qml
folder_01.target = qml
DEPLOYMENTFOLDERS = folder_01

# Additional import path used to resolve QML modules in Creator's code model
QML_IMPORT_PATH =

#PATH += "/qml/_Text.qml"

# The .cpp file which was generated for your project. Feel free to hack it.
SOURCES += main.cpp \
           src/cpp/CurveCalculator.cpp \
           src/cpp/graph.cpp \
    src/cpp/beziercurvehelper.cpp \
    src/cpp/peqgraph.cpp \
    src/cpp/dynamicpropertyhandler.cpp \
    src/cpp/dyncontroller.cpp \
    src/cpp/componentloader.cpp \
    src/cpp/newcontrol.cpp

# Installation path
# target.path =

# Please do not modify the following two lines. Required for deployment.
include(qtquick2applicationviewer/qtquick2applicationviewer.pri)
qtcAddDeployment()

OTHER_FILES += \
    src/qml/js/mainMenuScripts.js \
    src/qml/js/graphScripts.js \
    src/qml/PEQGraphView.qml

HEADERS += \
    src/cpp/graph.h \
    src/cpp/CurveCalculator.h \
    src/cpp/Constants.h \
    src/cpp/beziercurvehelper.h \
    src/cpp/peqgraph.h \
    src/cpp/dynamicpropertyhandler.h \
    src/cpp/dyncontroller.h \
    src/cpp/axialinfo.h \
    src/cpp/componentloader.h \
    src/cpp/himath.h \
    src/cpp/newcontrol.h

RESOURCES += \
    resources.qrc

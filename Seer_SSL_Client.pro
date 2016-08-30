TEMPLATE = app

QT += qml quick serialport
CONFIG += c++11

SOURCES +=

RESOURCES += qml.qrc

INCLUDEPATH += /usr/local/include

LIBS += /usr/local/lib/libprotobuf.dylib \
    /usr/local/lib/libboost_filesystem.dylib \
    /usr/local/lib/libboost_system.dylib

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Default rules for deployment.
include(deployment.pri)

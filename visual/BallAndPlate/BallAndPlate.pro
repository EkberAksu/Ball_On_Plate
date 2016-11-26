TEMPLATE = app

android|ios|tvos|winrt {
    warning( "This example is not supported for android, ios, tvos, or winrt." )
}


# Default rules for deployment.
!include( deployment.pri ) {
    error( "Couldn't find the deployment.pri file!" )
}

QT += 3dcore 3drender 3dinput 3dextras
QT += widgets
QT += qml quick

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

CONFIG += c++11

SOURCES += main.cpp \
    ball.cpp \
    plate.cpp

RESOURCES += qml.qrc

HEADERS += \
    ball.h \
    plate.h

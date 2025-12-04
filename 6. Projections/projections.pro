QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    areapaint.cpp \
    main.cpp \
    mainwindow.cpp \
    polynom.cpp \
    shape.cpp \
    shapes/composite.cpp \
    shapes/dot.cpp \
    shapes/line.cpp \
    shapes/polyedr.cpp \
    shapes/polygon.cpp \
    shapes/spline.cpp \
    shapes/splineermit.cpp \
    shapes/splinekinematic.cpp \
    shapes/surface.cpp \
    shapes/surfaceermit.cpp \
    shapes/surfaceinterpolated.cpp \
    shapes/surfacekinematic.cpp \
    vector.cpp

HEADERS += \
    areapaint.h \
    mainwindow.h \
    matrix.h \
    matrix_base.h \
    matrix_light.h \
    polynom.h \
    shape.h \
    vector.h


RESOURCES += \
    Resources.qrc

RC_ICONS = 3d-c.ico

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

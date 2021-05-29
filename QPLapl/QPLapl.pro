QT       += core gui \
            charts

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

QT += widgets \
      datavisualization

SOURCES += \
    chbessel.cpp \
    gridgraphwindow.cpp \
    gwell.cpp \
    interp_1d.cpp \
    linlogaxis.cpp \
    main.cpp \
    mainwindow.cpp \
    matrix3dv.cpp \
    noteditabledelegate.cpp \
    pqgraphwindow.cpp \
    qgaus.cpp \
    qgrid1d.cpp \
    surfacegraph.cpp \
    tablewellschedule.cpp \
    unicombobox.cpp \
    wellmanager.cpp \
    xymodel.cpp \
    picmanager.cpp

HEADERS += \
    auxillary.h \
    chbessel.h \
    gridgraphwindow.h \
    gwell.h \
    interp_1d.h \
    labelmapper.h \
    linlogaxis.h \
    mainwindow.h \
    matrix3dv.h \
    noteditabledelegate.h \
    pqgraphwindow.h \
    qgaus.h \
    qgrid1d.h \
    quadrature.h \
    surfacegraph.h \
    tablewellschedule.h \
    unicombobox.h \
    wellmanager.h \
    xymodel.h \
    picmanager.h

FORMS += \
    mainwindow.ui \
    pqgraphwindow.ui

RESOURCES += Vertical.jpg \
             Horizontal.jpg \
             Fractured.jpg \
             Multifractured.jpg \
             Blank.JPG

INCLUDEPATH += \
    C:/mingw64/mingw64/lib/gcc/x86_64-w64-mingw32/8.1.0/include

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

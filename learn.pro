QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

QMAKE_PROJECT_DEPTH = 0
QT += sql


CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    dlg_addstore.cpp \
    main.cpp \
    mainwindow.cpp \
    page_log.cpp \
    storesql.cpp

HEADERS += \
    dlg_addstore.h \
    mainwindow.h \
    page_log.h \
    storesql.h

FORMS += \
    dlg_addstore.ui \
    mainwindow.ui \
    page_log.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    res.qrc

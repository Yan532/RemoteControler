QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    Blockstruct.cpp \
    Controler.cpp \
    Controltee.cpp \
    Imagewidget.cpp \
    Socket.cpp \
    Startwidget.cpp \
    localhost.cpp \
    main.cpp \
    windows_eventapi.cpp

HEADERS += \
    Blockstruct.h \
    Controler.h \
    Controltee.h \
    Imagewidget.h \
    RemoteEvent.h \
    Socket.h \
    Startwidget.h \
    localhost.h \
    windows_eventapi.h

FORMS += \
    Imagewidget.ui \
    Startwidget.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

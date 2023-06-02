QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    sendimagedata.cpp \
    widget.cpp

HEADERS += \
    sendimagedata.h \
    widget.h

FORMS += \
    widget.ui

INCLUDEPATH += D:\opencv\build\include \
               D:\opencv\build\include\opencv2

LIBS += D:\opencv\build\x64\vc16\lib\opencv_world470.lib \
        D:\opencv\build\x64\vc16\lib\opencv_world470d.lib

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

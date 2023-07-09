QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    Model.cpp \
    frame.cpp \
    main.cpp \
    mainwindow.cpp \
    mydialog.cpp \
    preview.cpp

HEADERS += \
    Model.h \
    frame.h \
    mainwindow.h \
    mydialog.h \
    preview.h

FORMS += \
    frame.ui \
    mainwindow.ui \
    preview.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    ../../File Explorer/School Folder/Spring 2023/Software Practice II (CS 3505)/A7 Sprite Editor/Image Resources/Pen Icon.png

RESOURCES += \
    resources.qrc

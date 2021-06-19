#-------------------------------------------------
#
# Project created by QtCreator 2019-08-27T19:06:51
#
#-------------------------------------------------

QT       += core gui
QT += multimedia
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = AuTo
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0



SOURCES += \
        main.cpp \
    qdoublevalid.cpp \
    mouse_recorded_thread.cpp \
    keyboard_recorded_thread.cpp \
    util.cpp \
    global_data.cpp \
    replay_thread.cpp \
    setting.cpp \
    AuTo.cpp

HEADERS += \
    qdoublevalid.h \
    mouse_recorded_thread.h \
    keyboard_recorded_thread.h \
    global_data.h \
    util.h \
    replay_thread.h \
    setting.h \
    AuTo.h

FORMS += \
    setting.ui \
    AuTo.ui

RESOURCES += \
    pic.qrc \
    logo.qrc \
    style.qrc

DISTFILES += \
    logo.rc

RC_FILE += logo.rc

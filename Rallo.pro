#-------------------------------------------------
#
# Project created by QtCreator 2017-12-01T20:26:46
#
#-------------------------------------------------

QT       += core gui network uitools serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

win32 {
    greaterThan(QT_MAJOR_VERSION, 4): QT += multimedia
} else {
    DEFINES += ForRaspberryPi
}

TARGET = RalloApp
TEMPLATE = app

CONFIG += c++11 static # console

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
    argumentparser.cpp \
    beepsound.cpp \
    gpioclass.cpp \
    keyboardhandler.cpp \
    layout.cpp \
    modalwindow.cpp \
    rfid_rc522.cpp \
    serialcardread.cpp \
    serialporthandler.cpp \
    rallo.cpp

HEADERS += \
    argumentparser.h \
    beepsound.h \
    gpioclass.h \
    keyboardhandler.h \
    layout.h \
    modalwindow.h \
    rfid_rc522.h \
    serialcardread.h \
    serialporthandler.h \
    config.h \
    rallo.h

FORMS += \
    rallo.ui

SUBDIRS += \
    Rallo.pro

DISTFILES +=

RESOURCES += \
    resources/resources.qrc

DEFINES += new181

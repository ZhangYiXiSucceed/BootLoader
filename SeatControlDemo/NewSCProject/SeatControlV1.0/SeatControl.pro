#-------------------------------------------------
#
# Project created by QtCreator 2019-06-10T09:16:28
#
#-------------------------------------------------

QT       += core gui serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SeatControl
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        SCMainWindow.cpp \
    Login.cpp \
    SCParameter.cpp \
    SCButton.cpp \
    SCFirmwareUpgrade.cpp \
    SCRecordEvent.cpp \
    CRC_Cal.cpp

HEADERS += \
        SCMainWindow.h \
    Login.h \
    SCParameter.h \
    SCButton.h \
    SCFirmwareUpgrade.h \
    SCRecordEvent.h \
    CRC_Cal.h

FORMS += \
        SCMainWindow.ui \
    Login.ui \
    SCParameter.ui \
    SCButton.ui \
    SCFirmwareUpgrade.ui \
    SCRecordEvent.ui

RESOURCES += \
    image.qrc

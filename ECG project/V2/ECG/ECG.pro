#-------------------------------------------------
#
# Project created by QtCreator 2013-05-16T21:02:25
#
#-------------------------------------------------

QT       += core gui
QT       += core gui svg
QT       += network
QT       += xml
QT       += printsupport

#CONFIG   += console

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ECG
TEMPLATE = app


SOURCES += main.cpp\
        ecg.cpp \
    list.cpp \
    baselinewander.cpp \
    interpret.cpp

HEADERS  += ecg.h \
    list.h \
    baselinewander.h \
    interpret.h

FORMS    += ecg.ui \
    list.ui \
    interpret.ui

RESOURCES += \
    icon.qrc

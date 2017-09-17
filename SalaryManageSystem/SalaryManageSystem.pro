#-------------------------------------------------
#
# Project created by QtCreator 2017-08-17T09:44:28
#
#-------------------------------------------------

QT       += core gui
QT += sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SalaryManageSystem
TEMPLATE = app


RESOURCES += \
    res.qrc

FORMS += \
    mainwindow.ui \
    aboutdialog.ui \
    awardsdialog.ui \
    statdialog.ui \
    unlockdialog.ui

HEADERS += \
    mainwindow.h \
    aboutdialog.h \
    awardsdialog.h \
    statdialog.h \
    unlockdialog.h

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    aboutdialog.cpp \
    awardsdialog.cpp \
    statdialog.cpp \
    unlockdialog.cpp

RC_FILE = logo.rc

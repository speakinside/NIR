#-------------------------------------------------
#
# Project created by QtCreator 2017-12-31T00:19:41
#
#-------------------------------------------------

QT       += core gui charts serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = NIR
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
CONFIG += c++14
INCLUDEPATH += plog
SOURCES += \
        main.cpp \
    Core/deviceinterface.cpp \
    Core/datamodel.cpp \
    Core/savemethods.cpp \
    Gui/TableArea/sampletable.cpp \
    Gui/SpectraArea/spectraview.cpp \
    Gui/mainwindow.cpp \
    Gui/TabArea/controlpanel.cpp \
    Gui/TabArea/measurebox.cpp \
    Gui/TabArea/deviceconditionbox.cpp \
    Gui/TabArea/saveconfbox.cpp \
    Core/modeldelegate.cpp \
    Gui/SpectraArea/spectrachart.cpp \
    Gui/Dialogs/aboutdialog.cpp \
    Gui/Dialogs/measuredialog.cpp \
    Gui/Dialogs/connectdialog.cpp \
    Core/fakedevice.cpp

HEADERS += \
    Core/deviceinterface.h \
    Core/datamodel.h \
    Core/savemethods.h \
    Core/init.h \
    Gui/TableArea/sampletable.h \
    Gui/SpectraArea/spectraview.h \
    Gui/mainwindow.h \
    Gui/TabArea/controlpanel.h \
    Gui/TabArea/measurebox.h \
    Gui/TabArea/deviceconditionbox.h \
    Gui/TabArea/saveconfbox.h \
    Core/modeldelegate.h \
    Gui/SpectraArea/spectrachart.h \
    Gui/Dialogs/aboutdialog.h \
    Gui/Dialogs/measuredialog.h \
    Gui/Dialogs/connectdialog.h \
    Core/fakedevice.h

RESOURCES += \
    res.qrc

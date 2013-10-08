#-------------------------------------------------
#
# Project created by QtCreator 2013-09-12T07:18:06
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Crafter
TEMPLATE = app
DESTDIR = $$PWD/../../bin

RESOURCES = crafter.qrc

SOURCES += main.cpp\
        mainwindow.cpp \
    newprojectdialog.cpp \
    tileview.cpp \
    project.cpp \
    projectmodel.cpp \
    tileviewwindow.cpp \
    tiledelegate.cpp \
    tilemodel.cpp \
    tilemap.cpp \
    tileworld.cpp \
    tilemapmodel.cpp \
    tile.cpp \
    newlayerdialog.cpp \
    newworlddialog.cpp \
    projectpanel.cpp \
    layerpanel.cpp \
    tilespanel.cpp \
    dockpanel.cpp \
    stringinterface.cpp \
    boundprocessor.cpp \
    boundisland.cpp \
    resizelayerdialog.cpp \
    aboutdialog.cpp

HEADERS  += mainwindow.h \
    newprojectdialog.h \
    tileview.h \
    project.h \
    projectmodel.h \
    tileviewwindow.h \
    tiledelegate.h \
    tilemodel.h \
    tilemap.h \
    tileworld.h \
    tilemapmodel.h \
    tile.h \
    newlayerdialog.h \
    newworlddialog.h \
    projectpanel.h \
    layerpanel.h \
    tilespanel.h \
    dockpanel.h \
    stringinterface.h \
    boundprocessor.h \
    boundisland.h \
    resizelayerdialog.h \
    aboutdialog.h

FORMS    += mainwindow.ui \
    newprojectdialog.ui \
    tileview.ui \
    newlayerdialog.ui \
    newworlddialog.ui \
    projectpanel.ui \
    layerpanel.ui \
    tilespanel.ui \
    resizelayerdialog.ui \
    aboutdialog.ui

BaseDir = $$PWD/../build/workshop

Release:OBJECTS_DIR = $$BaseDir/release/.obj
Release:MOC_DIR = $$BaseDir/release/.moc
Release:RCC_DIR = $$BaseDir/release/.rcc
Release:UI_DIR = $$BaseDir/release/.ui

Debug:OBJECTS_DIR = $$BaseDir/debug/.obj
Debug:MOC_DIR = $$BaseDir/debug/.moc
Debug:RCC_DIR = $$BaseDir/debug/.rcc
Debug:UI_DIR = $$BaseDir/debug/.ui

BinDir = $$PWD/../../bin/
SrcDir = $$PWD/../../src

win32:CONFIG(release, debug|release): LIBS += -L$$BinDir -lCore
else:win32:CONFIG(debug, debug|release): LIBS += -L$$BinDir -lCored

win32:CONFIG(release, debug|release): LIBS += -L$$BinDir -lEngine
else:win32:CONFIG(debug, debug|release): LIBS += -L$$BinDir -lEngined

INCLUDEPATH += $$SrcDir
DEPENDPATH += $$SrcDir

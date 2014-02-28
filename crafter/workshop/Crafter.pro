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
    projectpanel.cpp \
    layerpanel.cpp \
    tilespanel.cpp \
    dockpanel.cpp \
    stringinterface.cpp \
    boundprocessor.cpp \
    boundisland.cpp \
    resizelayerdialog.cpp \
    aboutdialog.cpp \
    worldwizard_pageworld.cpp \
    worldwizard.cpp \
    worldwizard_pagefinalize.cpp \
    layerwizard_pagetileset.cpp \
    layerwizard_pageinfo.cpp \
    world/tileset.cpp \
    project/projecttreeitem.cpp \
    world/tilesetwriter.cpp \
    world/tilesetreader.cpp \
    world/tilefield.cpp \
    world/tileworldreader.cpp \
    world/tilemapdesc.cpp \
    world/tileworlddesc.cpp \
    world/tileworldimporter.cpp \
    world/tilebound.cpp \
    world/tileworldwriter.cpp \
    project/projecttreeworlditem.cpp \
    project/projecttreetextitem.cpp \
    resource.cpp \
    world/tileworldhandle.cpp \
    scriptview.cpp \
    script/yassyntaxhighlighter.cpp

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
    projectpanel.h \
    layerpanel.h \
    tilespanel.h \
    dockpanel.h \
    stringinterface.h \
    boundprocessor.h \
    boundisland.h \
    resizelayerdialog.h \
    aboutdialog.h \
    worldwizard_pageworld.h \
    worldwizard.h \
    worldwizard_pagefinalize.h \
    layerwizard_pagetileset.h \
    layerwizard_pageinfo.h \
    world/tileset.h \
    project/projecttreeitem.h \
    world/tilesetwriter.h \
    world/tilesetreader.h \
    world/tilefield.h \
    world/tileworldreader.h \
    world/tilemapdesc.h \
    world/tileworlddesc.h \
    world/tileworldimporter.h \
    world/tilebound.h \
    world/tileworldwriter.h \
    project/projecttreeworlditem.h \
    project/projecttreetextitem.h \
    resource.h \
    world/tileworldhandle.h \
    scriptview.h \
    script/yassyntaxhighlighter.h

FORMS    += mainwindow.ui \
    newprojectdialog.ui \
    tileview.ui \
    newlayerdialog.ui \
    projectpanel.ui \
    layerpanel.ui \
    tilespanel.ui \
    resizelayerdialog.ui \
    aboutdialog.ui \
    worldwizard_pageworld.ui \
    layerwizard_pagetileset.ui \
    layerwizard_pageinfo.ui \
    scriptview.ui

BaseDir = $$PWD/../build/workshop

Release:OBJECTS_DIR = $$BaseDir/release/.obj
Release:MOC_DIR = $$BaseDir/release/.moc
Release:RCC_DIR = $$BaseDir/release/.rcc
Release:UI_DIR = $$BaseDir/release/.ui

Debug:OBJECTS_DIR = $$BaseDir/debug/.obj
Debug:MOC_DIR = $$BaseDir/debug/.moc
Debug:RCC_DIR = $$BaseDir/debug/.rcc
Debug:UI_DIR = $$BaseDir/debug/.ui

BinDir = $$PWD/../../../bin/
SrcDir = $$PWD/../../../src

win32:CONFIG(release, debug|release): LIBS += -L$$BinDir -lCore
else:win32:CONFIG(debug, debug|release): LIBS += -L$$BinDir -lCored

win32:CONFIG(release, debug|release): LIBS += -L$$BinDir -lEngine
else:win32:CONFIG(debug, debug|release): LIBS += -L$$BinDir -lEngined

INCLUDEPATH += $$SrcDir
DEPENDPATH += $$SrcDir

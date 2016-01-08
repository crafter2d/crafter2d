#-------------------------------------------------
#
# Project created by QtCreator 2013-09-12T07:18:06
#
#-------------------------------------------------

QT       += core gui xml

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
    tilemapmodel.cpp \
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
    world/tileset.cpp \
    world/tilesetwriter.cpp \
    world/tilesetreader.cpp \
    world/tilefield.cpp \
    world/tileworldreader.cpp \
    world/tilemapdesc.cpp \
    world/tileworlddesc.cpp \
    world/tilebound.cpp \
    world/tileworldwriter.cpp \
    project/projecttreeitem.cpp \
    project/projecttreetextitem.cpp \
    resource.cpp \
    world/tileworldhandle.cpp \
    scriptview.cpp \
    script/yassyntaxhighlighter.cpp \
    script/scriptfile.cpp \
    script/scripthandle.cpp \
    project/projectbuilder.cpp \
    newscriptdialog.cpp \
    script/scriptbuilder.cpp \
    undosettile.cpp \
    undocleartile.cpp \
    project/projectrunner.cpp \
    outputpanel.cpp \
    project/projectmanager.cpp \
    tileselector.cpp \
    script/scriptlinenumberarea.cpp \
    tilesetmodel.cpp \
    resourcehandle.cpp \
    project/projecttreeobjectitem.cpp \
    project/projecttreerootitem.cpp \
    world/tileworld.cpp \
    world/tilemap.cpp \
    world/tile.cpp \
    newtilesetdialog.cpp \
    worldwizard_pagelayer.cpp \
    object/entity.cpp \
    object/entityreader.cpp \
    object/entitycomponent.cpp \
    object/spritecomponent.cpp \
    object/spriteanimation.cpp \
    object/physicscomponent.cpp

HEADERS  += mainwindow.h \
    newprojectdialog.h \
    tileview.h \
    project.h \
    projectmodel.h \
    tileviewwindow.h \
    tilemapmodel.h \
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
    world/tileset.h \
    project/projecttreeitem.h \
    world/tilesetwriter.h \
    world/tilesetreader.h \
    world/tilefield.h \
    world/tileworldreader.h \
    world/tilemapdesc.h \
    world/tileworlddesc.h \
    world/tilebound.h \
    world/tileworldwriter.h \
    project/projecttreetextitem.h \
    resource.h \
    world/tileworldhandle.h \
    scriptview.h \
    script/yassyntaxhighlighter.h \
    script/scriptfile.h \
    script/scripthandle.h \
    project/projectbuilder.h \
    newscriptdialog.h \
    script/scriptbuilder.h \
    undosettile.h \
    undocleartile.h \
    project/projectrunner.h \
    outputpanel.h \
    project/projectmanager.h \
    tileselector.h \
    script/scriptlinenumberarea.h \
    tilesetmodel.h \
    resourcehandle.h \
    project/projecttreeobjectitem.h \
    project/projecttreerootitem.h \
    world/tileworld.h \
    world/tilemap.h \
    world/tile.h \
    newtilesetdialog.h \
    worldwizard_pagelayer.h \
    object/entity.h \
    object/entityreader.h \
    object/entitycomponent.h \
    object/spritecomponent.h \
    object/spriteanimation.h \
    object/physicscomponent.h

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
    scriptview.ui \
    newscriptdialog.ui \
    outputpanel.ui \
    newtilesetdialog.ui \
    worldwizard_pagelayer.ui

BaseDir = $$PWD/../build/workshop

Release:OBJECTS_DIR = $$BaseDir/release/.obj
Release:MOC_DIR = $$BaseDir/release/.moc
Release:RCC_DIR = $$BaseDir/release/.rcc
Release:UI_DIR = $$BaseDir/release/.ui

Debug:OBJECTS_DIR = $$BaseDir/debug/.obj
Debug:MOC_DIR = $$BaseDir/debug/.moc
Debug:RCC_DIR = $$BaseDir/debug/.rcc
Debug:UI_DIR = $$BaseDir/debug/.ui

DebugBinDir = $$PWD/../../bin/
ReleaseBinDir = $$PWD/../../binrel/

SrcDir = $$PWD/../../src

win32:CONFIG(release, debug|release): LIBS += -L$$ReleaseBinDir -lCore
else:win32:CONFIG(debug, debug|release): LIBS += -L$$DebugBinDir -lCored

INCLUDEPATH += $$SrcDir
DEPENDPATH += $$SrcDir

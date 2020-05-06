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
    editentitydialog.cpp \
    mainwindow.cpp \
    newprojectdialog.cpp \
    project/projectdocument.cpp \
    project/projectsettings.cpp \
    projectsettingsdialog.cpp \
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
    boundprocessor.cpp \
    boundisland.cpp \
    resizelayerdialog.cpp \
    aboutdialog.cpp \
    world/tileentity.cpp \
    world/tileentityreader.cpp \
    world/tileentitywriter.cpp \
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
    newtilesetdialog.cpp \
    worldwizard_pagelayer.cpp \
    entity/entity.cpp \
    entity/entityreader.cpp \
    entity/entitycomponentregistry.cpp \
    entity/entitycomponent.cpp \
    entity/spritecomponent.cpp \
    entity/spriteanimation.cpp \
    entity/physicscomponent.cpp \
    texture/texturesplitter.cpp \
    texturepacker/texturepacker.cpp \
    texturepacker/texturepackerview.cpp \
    texturepacker/maxrectsalgorithm.cpp \
    texturepacker/sprite.cpp \
    texturepacker/spritesheet.cpp \
    texturepacker/spriteatlas.cpp \
    texturepacker/spritesheetwriter.cpp \
    texturepacker/spritesheetreader.cpp \
    helpers/rotateimagepainter.cpp \
    tilepropertiesdialog.cpp \
    newentitydialog.cpp

HEADERS  += mainwindow.h \
    editentitydialog.h \
    newprojectdialog.h \
    project/projectdocument.h \
    project/projectsettings.h \
    projectsettingsdialog.h \
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
    boundprocessor.h \
    boundisland.h \
    resizelayerdialog.h \
    aboutdialog.h \
    world/tileentity.h \
    world/tileentityreader.h \
    world/tileentitywriter.h \
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
    newtilesetdialog.h \
    worldwizard_pagelayer.h \
    entity/entity.h \
    entity/entityreader.h \
    entity/entitycomponent.h \
    entity/spritecomponent.h \
    entity/spriteanimation.h \
    entity/physicscomponent.h \
    entity/entitycomponentregistry.h \
    texture/texturesplitter.h \
    texturepacker/texturepacker.h \
    texturepacker/texturepackerview.h \
    texturepacker/maxrectsalgorithm.h \
    texturepacker/sprite.h \
    texturepacker/spritesheet.h \
    texturepacker/spriteatlas.h \
    texturepacker/spritesheetwriter.h \
    texturepacker/spritesheetreader.h \
    helpers/rotateimagepainter.h \
    tilepropertiesdialog.h \
    newentitydialog.h

FORMS    += mainwindow.ui \
    editentitydialog.ui \
    newprojectdialog.ui \
    projectsettingsdialog.ui \
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
    worldwizard_pagelayer.ui \
    texture/texturesplitter.ui \
    texturepacker/texturepacker.ui \
    tilepropertiesdialog.ui \
    newentitydialog.ui

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

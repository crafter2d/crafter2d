#-------------------------------------------------
#
# Project created by QtCreator 2013-10-07T14:52:11
#
#-------------------------------------------------

QT       += core gui

TARGET = DDSHandler
TEMPLATE = lib
CONFIG += plugin

DESTDIR = $$[QT_INSTALL_PLUGINS]/imageformats

SOURCES += \
    ddsplugin.cpp \
    ddshandler.cpp

HEADERS += \
    ddsplugin.h \
    ddshandler.h

OTHER_FILES += DDSPlugin.json

unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/lib
    }
    INSTALLS += target
}

BaseDir = $$PWD/../build/ddsplugin

Release:OBJECTS_DIR = $$BaseDir/release/.obj
Release:MOC_DIR = $$BaseDir/release/.moc
Release:RCC_DIR = $$BaseDir/release/.rcc
Release:UI_DIR = $$BaseDir/release/.ui

Debug:OBJECTS_DIR = $$BaseDir/debug/.obj
Debug:MOC_DIR = $$BaseDir/debug/.moc
Debug:RCC_DIR = $$BaseDir/debug/.rcc
Debug:UI_DIR = $$BaseDir/debug/.ui

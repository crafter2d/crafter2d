#include "worldwizard.h"

#include <QDir>
#include <QFile>
#include <QMessageBox>
#include <QVariant>

#include "worldwizard_pagelayer.h"
#include "worldwizard_pageworld.h"
#include "worldwizard_pagefinalize.h"

#include "project/projectmanager.h"

#include "world/tilemap.h"
#include "world/tileworld.h"
#include "world/tileset.h"
#include "world/tilemapdesc.h"
#include "world/tileworlddesc.h"

#include "project.h"

// static
TileWorld* WorldWizard::show()
{
    TileWorld* presult = nullptr;

    WorldWizard wizard;
    if ( wizard.exec() == QDialog::Accepted )
    {
        presult = wizard.createWorld();
    }

    return presult;
}

WorldWizard::WorldWizard(QWidget *parent) :
    QWizard(parent)
{
    setPage(Page_World, new WorldWizard_PageWorld());
    setPage(Page_Layer, new WorldWizard_PageLayer());
    setPage(Page_Finalize, new WorldWizard_PageFinalize());

    setStartId(Page_World);

#ifndef Q_OS_MAC
    setWizardStyle(ModernStyle);
#endif

    setWindowTitle(tr("New World Wizard"));
}

// Handle dynamic navigation through this wizard here
// as we are re-using parts of the Layer wizard as well.
int WorldWizard::nextId() const
{
    switch ( currentId() )
    {
    case Page_World:
        return Page_Layer;

    case Page_Layer:
        return Page_Finalize;

    case Page_Finalize:
        break;
    }

    return -1;
}

// - Operations

TileWorld *WorldWizard::createWorld()
{
    TileWorldDesc desc;
    desc.name = field("world.name").toString();
    desc.layerType = field("world.isTopDown").toBool() ? TileWorld::eTopDown : TileWorld::eIsoMetric;

    TileWorld* pworld = new TileWorld(desc);
    pworld->setResourceName("worlds/" + desc.name + ".world");
    pworld->setDirty(true);

    if ( field("layer.create").toBool() )
    {
        int width = field("layer.width").toString().toInt();
        int height = field("layer.height").toString().toInt();

        TileMapDesc mapdesc;
        mapdesc.name = field("layer.name").toString();
        mapdesc.effect = "shaders/basic";
        mapdesc.size = QSize(width, height);

        QString tileset = field("layer.tileset").toString();
        if ( tileset != "none" )
        {
            mapdesc.tileset = tileset;// ptileset->getResourceName();
            mapdesc.flags = TileMapDesc::eTileSet;
        }
        else
        {
            mapdesc.flags = TileMapDesc::eObjects;
        }

        TileMap* pmap = new TileMap(mapdesc);
        pworld->addMap(pmap);
    }

    return pworld;
}

TileSet* WorldWizard::createTileSet()
{
    QString imagefile = field("tileset.image").toString();
    QFileInfo info(imagefile);

    Project& project = Project::getActiveProject();
    QString finalpath = project.getBasePath() + QDir::separator() + "images" + QDir::separator() + info.fileName();
    if ( finalpath != imagefile )
    {
        bool copyit = true;
        if ( QFile::exists(finalpath) )
        {
            QMessageBox::StandardButton result =
                    QMessageBox::warning(this, "Crafter Workshop",
                                         "There is already an image with that name.\nDo you want to overwrite the existing file?",
                                         QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
            if ( result == QMessageBox::Cancel )
            {
                return nullptr;
            }

            copyit = result == QMessageBox::Yes;
        }

        if ( copyit && !QFile::copy(imagefile, finalpath) )
        {
            QMessageBox::critical(this, "Crafter Workshop", "Could not copy file to demo folder.", QMessageBox::Ok);
            return nullptr;
        }
    }

    TileSet* presult = new TileSet();
    presult->setResourceName("tilesets/" + field("tileset.name").toString() + ".xml");
    presult->setDirty(true);

    project.addTileSet(presult);

    return presult;
}

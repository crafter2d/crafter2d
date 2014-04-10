#include "worldwizard.h"

#include <QDir>
#include <QFile>
#include <QMessageBox>
#include <QVariant>

#include "core/smartptr/autoptr.h"

#include "layerwizard_pageinfo.h"
#include "layerwizard_pagetileset.h"
#include "worldwizard_pageworld.h"
#include "worldwizard_pagefinalize.h"

#include "stringinterface.h"
#include "tileworld.h"
#include "tilemap.h"
#include "project.h"

#include "world/tileset.h"
#include "world/tilemapdesc.h"
#include "world/tileworlddesc.h"

// static
TileWorld* WorldWizard::show()
{
    TileWorld* presult = NULL;

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
    setPage(Page_Layer, new LayerWizard_PageInfo());
    setPage(Page_TileSet, new LayerWizard_PageTileSet());
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
        if ( field("layer.create").toBool() )
            return Page_TileSet;
        else
            return Page_Finalize;

    case Page_TileSet:
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
        QTileSet* ptileset = nullptr;

        if ( field("tileset.create").toBool() )
        {
            // create a new tileset
            ptileset = createTileSet();
        }
        else
        {
            // select existing tileset
        }

        int width = field("layer.width").toString().toInt();
        int height = field("layer.height").toString().toInt();

        TileMapDesc mapdesc;
        mapdesc.name = field("layer.name").toString();
        mapdesc.tileset = ptileset->getResourceName();
        mapdesc.effect = "shaders/basic";
        mapdesc.size = QSize(width, height);

        TileField* pfield = new TileField();
        pfield->create(QSize(width, height));

        TileMap* pmap = new TileMap(mapdesc);
        pmap->setTileSet(ptileset);
        pmap->setField(pfield);
        pworld->addMap(pmap);
    }

    return pworld;
}

QTileSet* WorldWizard::createTileSet()
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
        }
    }

    int width = field("tileset.width").toString().toInt();
    int height = field("tileset.height").toString().toInt();
    int count = field("tileset.count").toString().toInt();

    AutoPtr<QTileSet> presult = new QTileSet();
    presult->setResourceName("tilesets/" + field("tileset.name").toString() + ".xml");
    presult->setTileMap("images/" + info.baseName());
    presult->setTileSize(QSize(width, height));
    presult->setTileCount(count);
    presult->setDirty(true);

    project.addTileSet(presult.release());

    return presult.getPointer();
}

#include "worldwizard.h"

#include <QVariant>

#include "layerwizard_pageinfo.h"
#include "layerwizard_pagetileset.h"
#include "worldwizard_pageworld.h"
#include "worldwizard_pagefinalize.h"

#include "stringinterface.h"
#include "tileworld.h"
#include "tilemap.h"

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


    if ( field("layer.create").toBool() )
    {
        if ( field("tileset.create").toBool() )
        {
            // create a new tileset
        }
        else
        {
            // select existing tileset
        }

        TileMapDesc mapdesc;
        mapdesc.name = field("layer.name").toString();

        TileMap* pmap = new TileMap(mapdesc);
        pworld->addMap(pmap);
    }

    return pworld;
}

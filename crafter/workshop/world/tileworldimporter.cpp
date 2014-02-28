#include "tileworldimporter.h"

#include <QFileInfo>
#include <QFileDialog>
#include <QMessageBox>
#include <QString>

#include <core/smartptr/autoptr.h>
#include <core/string/string.h>
#include <core/physics/simulationfactory.h>
#include <core/physics/simulationfactoryregistry.h>

#include <core/world/bound.h>
#include <core/world/bounds.h>
#include <core/world/layer.h>
#include <core/world/world.h>
#include <core/world/worldreader.h>

#include "../project.h"
#include "../tilemap.h"
#include "../tileworld.h"
#include "../stringinterface.h"

#include "tilebound.h"
#include "tilefield.h"
#include "tileset.h"
#include "tilesetreader.h"

// static
TileWorld* QTileWorldImporter::import(QWidget* pparent, const QString& filename)
{
    QTileWorldImporter importer(pparent);
    return importer.importWorld(filename);
}

// - Class Implementation

QTileWorldImporter::QTileWorldImporter(QWidget* pparent):
    mpParent(pparent),
    mBaseDir()
{
}

TileWorld* QTileWorldImporter::importWorld(const QString& filename)
{
    QFileInfo info(filename);
    mBaseDir = info.absoluteDir();
    mBaseDir.cdUp();

    if ( filename.contains("jwl") )
    {
        return importOldFormat(filename);
    }
    else
    {
        return NULL;
    }
}

TileWorld* QTileWorldImporter::importOldFormat(const QString& filename)
{
    AutoPtr<World> world = new World();

    WorldReader reader;
    String path = toString(filename);
    if ( !reader.read(*world, path) )
    {
        return NULL;
    }

    SimulationFactory* pfactory = SimulationFactoryRegistry::getInstance().findFactory(world->getSimulator());
    if ( pfactory == NULL )
    {
        return NULL;
    }

    TileWorldDesc desc;
    desc.version = 1;
    desc.name = toQString(world->getName());
    desc.simulator = toQString(pfactory->getName());
    desc.layerType = world->getLayerType() == LayerType::ETopDown ? TileWorld::eTopDown : TileWorld::eIsoMetric;

    if ( desc.name.isNull() || desc.name.isEmpty() )
    {
        // make sure that we have a name
        QFileInfo info(filename);
        desc.name = info.baseName();
    }

    TileWorld* pworld = new TileWorld(desc);
    pworld->setResourceName(QString("worlds/") + desc.name + QString(".world"));
    pworld->setDirty(true);

    for ( int index = 0; index < world->getLayerCount(); ++index )
    {
        Layer* player = world->getLayer(index);
        TileMap* pmap = importMap(*player);
        pworld->addMap(pmap);
    }

    const Bounds& bounds = world->getBounds();
    for ( std::size_t index = 0; index < bounds.size(); ++index )
    {
        const Bound& bound = *bounds[index];
        TileBound* pbound = new TileBound();
        pbound->setLeft(QPoint(bound.getLeft().x, bound.getLeft().y));
        pbound->setRight(QPoint(bound.getRight().x, bound.getRight().y));
        pworld->addBound(pbound);
    }

    if ( pworld->getMapCount() > 0 )
    {
        pworld->setActiveMap(pworld->getMap(0));
    }

    return pworld;
}

TileMap* QTileWorldImporter::importMap(Layer& layer)
{
    const LayerDefinition& def = layer.getDefinition();

    TileMapDesc desc;
    desc.name = toQString(def.name);
    desc.effect = toQString(def.effect);
    desc.tileset = toQString(def.tileset);
    desc.size = QSize(def.width, def.height);

    // lookup the tileset
    QTileSet* ptileset = Project::getActiveProject().lookupTileSet(desc.tileset);
    if ( ptileset == NULL )
    {
        ptileset = importTileSet(desc.tileset);
        if ( ptileset == NULL )
        {
            // user pressed cancel
            return NULL;
        }

        // update the name of the tileset to match it
        desc.tileset = ptileset->getResourceName();
    }

    AutoPtr<TileMap> map = new TileMap(desc);
    map->setTileSet(ptileset);

    // import the field
    QTileField* pfield = new QTileField();
    pfield->create(desc.size);
    map->setField(pfield);

    for ( int y = 0; y < def.height; ++y )
    {
        for ( int x = 0; x < def.width; ++x )
        {
            quint8 tile = layer.getTile(LayerLevel::eBack, x, y);
            pfield->set(QTileField::eBack, x, y, tile);

            tile = layer.getTile(LayerLevel::eMid, x, y);
            pfield->set(QTileField::eMid, x, y, tile);

            tile = layer.getTile(LayerLevel::eFront, x, y);
            pfield->set(QTileField::eFront, x, y, tile);
        }
    }

    return map.release();
}

QTileSet* QTileWorldImporter::importTileSet(const QString& tileset)
{
    QTileSet* presult = NULL;

    // in the world files the tileset was saved only by name.
    // originally tilesets where situated in the image folder.
    QString path = QString("images/") + tileset + QString(".xml");

    if ( mBaseDir.exists(path) )
    {
        QString fullpath = mBaseDir.filePath(path);
        presult = loadTileSet(fullpath);
    }
    else
    {
        QMessageBox::StandardButton result;
        result = QMessageBox::warning(mpParent,
                                      "Missing tileset",
                                      "Could not find tileset '"  + tileset + "' for this map.\n"
                                      "Please press Ok for manual selection or Cancel to abort.",
                                      QMessageBox::Ok | QMessageBox::Cancel);

        if ( result == QMessageBox::Ok )
        {
            presult = selectTileSet(tileset);
        }
    }

    if ( presult != NULL )
    {
        // register the tilset with the project
        Project::getActiveProject().addTileSet(presult);
    }

    return presult;
}

QTileSet* QTileWorldImporter::selectTileSet(const QString& name)
{
    QTileSet* ptileset = NULL;
    QString filename = QFileDialog::getOpenFileName(mpParent, "Select tileset " + name, QString(), "TileSet (*.xml)");
    if ( !filename.isNull() )
    {
        ptileset = loadTileSet(filename);
        if ( ptileset == NULL )
        {
            QMessageBox::StandardButton result;
            result = QMessageBox::warning(mpParent,
                                          "Failed",
                                          "Could not load the tileset from " + filename + ". Press Ok to try again.",
                                          QMessageBox::Ok | QMessageBox::Cancel);
            if ( result == QMessageBox::Ok )
            {
                ptileset = selectTileSet(name);
            }
        }
    }

    return ptileset;
}

QTileSet* QTileWorldImporter::loadTileSet(const QString& filename)
{
    QTileSet* ptileset = NULL;
    QFile file(filename);

    if ( file.open(QIODevice::ReadOnly | QIODevice::Text) )
    {
        TileSetReader reader(file);
        ptileset = reader.read();

        if ( ptileset != NULL )
        {
            if ( !ptileset->hasTexture() )
            {
                QString path = importTexture(ptileset->getTileMap());
                ptileset->setTileMap(path, true);
            }

            ptileset->setResourceName(QString("tilesets/") + QFileInfo(file).fileName());
            ptileset->setDirty(true);
        }
    }

    return ptileset;
}

QString QTileWorldImporter::importTexture(const QString& filename)
{
    QString texturepath;
    QFileInfo info(filename);
    if ( !info.isAbsolute() ) // not much we can do with an absolute path, if it exists it should have already been loaded
    {
        // here we assume that the old file structure is used: ...\crafter2d\image\tileimage.dds
        // normally the image is specified from the crafter2d base directory

        if ( mBaseDir.exists(filename) )
        {
            texturepath = QString("images/") + info.fileName();
        }
        else
        {
            QMessageBox::StandardButton result;
            result = QMessageBox::warning(mpParent,
                                          "Missing image",
                                          "Can not find image '" + info.fileName() + "'. Press Ok to search manually or Cancel to skip this step.",
                                          QMessageBox::Ok | QMessageBox::Cancel);
            if ( result == QMessageBox::Ok )
            {
                texturepath = QFileDialog::getOpenFileName(mpParent, "Select image " + info.fileName());
            }
        }

        if ( !texturepath.isEmpty() )
        {
            QString newpath = Project::getActiveProject().getFilePath(texturepath);
            QFile::copy(mBaseDir.filePath(filename), newpath);
        }
    }

    return texturepath;
}

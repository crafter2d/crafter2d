
#include "worldreader.h"

#include <zlib.h>

#include "core/defines.h"
#include "core/content/contentmanager.h"
#include "core/physics/simulationfactory.h"
#include "core/physics/simulationfactoryregistry.h"
#include "core/streams/datastream.h"
#include "core/string/string.h"
#include "core/world/bound.h"
#include "core/world/layer.h"
#include "core/world/world.h"

IContent* WorldReader::read(DataStream& stream)
{
   // read the header
   int version, layertype;
   String name, simulator;
   stream >> version >> name >> simulator >> layertype;

   SimulationFactory* pfactory = SimulationFactoryRegistry::getInstance().findFactory(simulator);

   World* pworld = new World();
   pworld->setName(name);
   pworld->setLayerType(layertype);
   
   int layercount;
   stream >> layercount;
   for ( int index = 0; index < layercount; ++index )
   {
      // read layer header
      String layername, effect, tileset;
      stream >> layername >> effect >> tileset;

      // read the field data
      uint32_t datasize;
      int width, height;
      stream >> width >> height;
      stream.readUint32(datasize);
      uint8_t* pdata = new uint8_t[datasize];
      stream.readBlob(pdata, datasize);

      // we are importing the QByteArray data here, so we use
      // the algorithm as used by Qt5

      // get the expected length from the data
      uint32_t explen = (pdata[0] << 24) | (pdata[1] << 16) |
                        (pdata[2] <<  8) | (pdata[3]      );
      uint64_t len = explen;

      // decompress the field data
      uint8_t* puncompressed = new uint8_t[explen];
      uncompress(puncompressed, &len, pdata + 4, datasize - 4);
      delete[] pdata;

      LayerDefinition* pdefinition = new LayerDefinition();
      pdefinition->effect = effect;
      pdefinition->tileset = tileset;
      pdefinition->name = name;
      pdefinition->width = width;
      pdefinition->height = height;

      TileSet* ptileset = getContentManager().loadContent<TileSet>(tileset);

      Layer* player = pworld->createLayer();
      player->setTileSet(ptileset);
      player->create(pdefinition);      
      pworld->addLayer(player);

      TileField& field = player->getTileField();
      field.create(width, height, puncompressed);
   }

   int boundCount;
   stream >> boundCount;
   for ( int index = 0; index < boundCount; ++index )
   {
      Vector left, right;
      stream >> left >> right;
      pworld->addBound(left, right);
   }

   // needs to be set here, otherwise the bounds are not added to the simulation
   // to be changed that new bounds are added automatically to the simulation as well
   pworld->setSimulator(pfactory->createSimulator());

   return pworld;
}


#include "worldreader.h"

#include <stdexcept>
#include <zlib.h>

#include "core/defines.h"
#include "core/entity/entity.h"
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
      stream >> width >> height >> datasize;
      if ( datasize < 4 )
         throw std::length_error("Expected at least 4 bytes, but got less.");

      auto pdata = std::make_unique<uint8_t[]>(datasize);
      stream.readBlob(pdata.get(), datasize);

      // we are importing the QByteArray data here, so we use
      // the algorithm as used by Qt5

      // get the expected length from the data
      uint32_t explen = (pdata[0] << 24) | (pdata[1] << 16) |
                        (pdata[2] <<  8) | (pdata[3]      );
      uLongf len = explen;
      if ( len > 0 )
      {
         // decompress the field data
         auto puncompressed = std::make_unique<uint8_t[]>(explen);
         int result = uncompress(puncompressed.get(), &len, pdata.get() + 4, datasize - 4);
         if ( result != Z_OK )
            throw std::runtime_error("Could not decompress the layer information.");

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
         field.create(width, height, puncompressed.release());
      }
   }

   int boundCount;
   stream >> boundCount;
   for ( int index = 0; index < boundCount; ++index )
   {
      Vector left, right;
      stream >> left >> right;
      pworld->addBound(left, right);
   }

   bool hasEntities = false;
   stream >> hasEntities;
   if ( hasEntities )
   {
      uint32_t count;
      stream >> count;
      for ( uint32_t index = 0; index < count; ++index )
      {
         String name;
         float x, y, scale;
         stream >> name >> x >> y >> scale;

         Entity* pentity = getContentManager().loadContent<Entity>(name);
         pentity->setPosition(Vector(x, y));
         pentity->scale(scale);

         pworld->addEntity(pentity);
      }
   }

   // needs to be set here, otherwise the bounds are not added to the simulation
   // to be changed that new bounds are added automatically to the simulation as well
   pworld->setSimulator(pfactory->createSimulator());

   return pworld;
}

/***************************************************************************
 *   Copyright (C) 2006 by Jeroen Broekhuizen                              *
 *   jengine.sse@live.nl                                                   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU Library General Public License as       *
 *   published by the Free Software Foundation; either version 2 of the    *
 *   License, or (at your option) any later version.                       *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU Library General Public     *
 *   License along with this program; if not, write to the                 *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#include "worldversion2reader.h"

#include <tinyxml.h>

#include "core/string/string.h"
#include "core/vfs/unzipfile.h"
#include "core/streams/arraystream.h"

#include "engine/physics/simulationfactoryregistry.h"

#include "bound.h"
#include "layer.h"
#include "world.h"
#include "worldwriter.h"

DataStream& operator>>(DataStream& in, Layer& layer);

//////////////////////////////////////////////////////////////////////////
// - Static operations
//////////////////////////////////////////////////////////////////////////

int WorldVersion2Reader::getVersion()
{
   return WorldWriter::getVersion();
}

//////////////////////////////////////////////////////////////////////////
// - Construction
//////////////////////////////////////////////////////////////////////////

WorldVersion2Reader::WorldVersion2Reader(World& world):
   AbstractWorldVersionReader(world)
{
}

WorldVersion2Reader::~WorldVersion2Reader()
{
}

//////////////////////////////////////////////////////////////////////////
// - Overloads
//////////////////////////////////////////////////////////////////////////

bool WorldVersion2Reader::virRead()
{
   UnzipFile zip(getFilename());

   return readHeader(zip)
       && readLayers(zip)
       && readBounds(zip)
       && readSimulator(zip);
       //&& readObjects(zip);
}

bool WorldVersion2Reader::readHeader(UnzipFile& zip)
{
   int size;
   char* pdata = NULL;

   if ( !zip.readFile(UTEXT("header"), (void*&)pdata, size) )
      return false;

   int version = -1;
   ArrayStream stream(pdata, size);
   stream.readInt(version);

   delete[] pdata;

   return getVersion() == version;
}

bool WorldVersion2Reader::readSimulator(UnzipFile& zip)
{
   TiXmlDocument doc;

   if ( loadXmlFromZip(zip, doc, UTEXT("simulator.xml")) )
   {
      TiXmlElement* psimulator = doc.FirstChildElement("simulator");
      if ( psimulator == NULL )
      {
         // illegal file format
         return false;
      }

      std::string type = psimulator->Attribute("type");

      SimulationFactory* pfactory = SimulationFactoryRegistry::getInstance().findFactory(type);
      if ( pfactory == NULL )
      {
         // simulator factory is not known
         return false;
      }

      getWorld().setSimulationFactory(*pfactory);
   }

   return true;
}

bool WorldVersion2Reader::readLayers(UnzipFile& zip)
{
   int size;
   char* pdata = NULL;

   if ( !zip.readFile(UTEXT("data"), (void*&)pdata, size) )
      return false;

   ArrayStream stream(pdata, size);

   int layertype = 0;
   int layercount = 0;
   stream >> layertype >> layercount;
   getWorld().setLayerType(layertype);

   for ( int index = 0; index < layercount; ++index )
   {
      readLayer(stream);
   }

   delete[] pdata;

   return true;
}

bool WorldVersion2Reader::readBounds(UnzipFile& zip)
{
   TiXmlDocument doc;

   if ( loadXmlFromZip(zip, doc, UTEXT("bounds.xml")) )
   {
      TiXmlElement* pbounds = doc.FirstChildElement("bounds");
      TiXmlElement* pelement = pbounds->FirstChildElement("bound");
      Vector left, right;

      while ( pelement != NULL )
      {
         pelement->QueryFloatAttribute("left_x", &left.x);
         pelement->QueryFloatAttribute("left_y", &left.y);
         pelement->QueryFloatAttribute("right_x", &right.x);
         pelement->QueryFloatAttribute("right_y", &right.y);

         getWorld().addBound(left, right);

         pelement = pelement->NextSiblingElement();
      }

      return true;
   }

   return false;
}

bool WorldVersion2Reader::readObjects(UnzipFile& zip)
{
   TiXmlDocument doc;

   if ( loadXmlFromZip(zip, doc, UTEXT("objects.xml")) )
   {
      return true;
   }
   return false;
}

//////////////////////////////////////////////////////////////////////////
// - Helpers
//////////////////////////////////////////////////////////////////////////

bool WorldVersion2Reader::loadXmlFromZip(UnzipFile& zip, TiXmlDocument& doc, const String& filename)
{
   int size;
   char* pdata = NULL;

   if ( !zip.readFile(filename, (void*&)pdata, size) )
      return false;

   std::stringstream stream;
   stream << pdata;

   stream >> doc;

   return !doc.Error();
}

//////////////////////////////////////////////////////////////////////////
// - Layer stream functions
//////////////////////////////////////////////////////////////////////////

void WorldVersion2Reader::readLayer(DataStream& in)
{
   int width, height;
   String name, effect, tileset;

   Layer* player = getWorld().createLayer();
   getWorld().addLayer(player);

   //in >> name >> effect >> width >> height;
   //tileset = UTEXT("blocks");

   in >> name >> effect >> tileset >> width >> height;

   LayerDefinition* pdefinition = new LayerDefinition();
   pdefinition->effect = effect;
   pdefinition->tileset = tileset;
   pdefinition->name = name;
   pdefinition->width = width;
   pdefinition->height = height;

   player->create(pdefinition);

   for ( int y = 0; y < height; ++y )
   {
      for ( int x = 0; x < width; ++x )
      {
         int textureid;
         in.readInt(textureid);
         player->setTile(x, y, textureid);
      }
   }
}

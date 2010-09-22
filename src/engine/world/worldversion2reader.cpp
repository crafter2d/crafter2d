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

#include "physics/simulationfactoryregistry.h"

#include "../vfs/unzipfile.h"

#include "bound.h"
#include "layer.h"
#include "world.h"
#include "worldwriter.h"

BitStream& operator>>(BitStream& in, Layer& layer);

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
       && readSimulator(zip)
       && readLayers(zip)
       && readBounds(zip);
       //&& readObjects(zip);
}

bool WorldVersion2Reader::readHeader(UnzipFile& zip)
{
   int size;
   char* pdata = NULL;

   if ( !zip.readFile("header", (void*&)pdata, size) )
      return false;

   BitStream stream;
   stream.setBuffer(pdata, size);

   delete[] pdata;
   pdata = NULL;

   int version = stream.readInt();
   return getVersion() == version;
}

bool WorldVersion2Reader::readSimulator(UnzipFile& zip)
{
   TiXmlDocument doc;

   if ( loadXmlFromZip(zip, doc, "simulator.xml") )
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

   if ( !zip.readFile("data", (void*&)pdata, size) )
      return false;

   BitStream stream;
   stream.setBuffer(pdata, size);

   delete[] pdata;

   int layertype = 0;
   int layercount = 0;
   stream >> layertype >> layercount;
   getWorld().setLayerType(layertype);

   for ( int index = 0; index < layercount; ++index )
   {
      Layer* player = getWorld().createLayer();
      getWorld().addLayer(player);

      stream >> *player;
   }

   return true;
}

bool WorldVersion2Reader::readBounds(UnzipFile& zip)
{
   TiXmlDocument doc;

   if ( loadXmlFromZip(zip, doc, "bounds.xml") )
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

   if ( loadXmlFromZip(zip, doc, "objects.xml") )
   {
      return true;
   }
   return false;
}

//////////////////////////////////////////////////////////////////////////
// - Helpers
//////////////////////////////////////////////////////////////////////////

bool WorldVersion2Reader::loadXmlFromZip(UnzipFile& zip, TiXmlDocument& doc, const std::string& filename)
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

BitStream& operator>>(BitStream& in, Layer& layer)
{
   Vector dimensions, scroll;

   std::string name = in.readString();
   std::string effect = in.readString();

   in >> dimensions;

   layer.create(name, dimensions.x, dimensions.y, effect);

   for ( int y = 0; y < dimensions.y; ++y )
   {
      for ( int x = 0; x < dimensions.x; ++x )
      {
         int textureid = in.readInt();
         layer.setTile(x, y, textureid);
      }
   }

   return in;
}
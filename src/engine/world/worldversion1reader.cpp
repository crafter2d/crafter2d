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
#include "worldversion1reader.h"

#include <iostream>

#include "../console.h"

#include "bound.h"
#include "layer.h"
#include "layerfactory.h"
#include "world.h"

WorldVersion1Reader::WorldVersion1Reader(World& world):
   AbstractWorldVersionReader(world)
{
}

WorldVersion1Reader::~WorldVersion1Reader()
{
}

//////////////////////////////////////////////////////////////////////////
// - Operations
//////////////////////////////////////////////////////////////////////////

bool WorldVersion1Reader::virRead()
{
   std::ifstream stream (getFilename().c_str(), std::ios_base::in | std::ios_base::binary);
   if ( !stream.is_open () )
   {
      Console::getInstance().printf("World:create : Can not open file %s", getFilename().c_str());
      return false;
   }

   // read in layer type
   int layertype = 0;
   stream.read((char*)&layertype, sizeof(int));
   getWorld().setLayerType(layertype);

   return readLayers(stream) && readBounds(stream);
}

bool WorldVersion1Reader::readLayers(std::ifstream& stream)
{
   // read in the layers
   int layercount = 0;
   stream.read ((char*)&layercount, sizeof (int));

   for ( int i = 0; i < layercount; ++i )
   {
      Layer* player = createLayer();
      getWorld().addLayer(player);

      if ( !readLayer(stream, *player) || !player->prepare() )
      {
         Console::getInstance().printf("World:create : loading layer %d from %s failed.", i, getFilename().c_str());
         getWorld().destroy ();
         return false;
      }
   }

   return true;
}

bool WorldVersion1Reader::readLayer(std::ifstream& stream, Layer& layer)
{
   unsigned char nameLen = 0;
	char buffer[255];

   int width, height, tileWidth, tileHeight, tileCount, scrollSpeedX, scrollSpeedY;
   std::string name, effectName;

	memset (buffer, 0, 255);
	stream.read ((char*)&nameLen, sizeof(unsigned char));
	stream.read (buffer, nameLen);
   layer.setName(buffer);

	// read in the dimensions
	stream.read ((char*)&width, sizeof(int));
	stream.read ((char*)&height, sizeof(int));
	stream.read ((char*)&tileWidth, sizeof (int));
	stream.read ((char*)&tileHeight, sizeof (int));
	stream.read ((char*)&tileCount, sizeof (int));

	// read in the effect file name
	memset (buffer, 0, 255);
	stream.read ((char*)&nameLen, sizeof (unsigned char));
	stream.read (buffer, nameLen);
	effectName = buffer;

   std::size_t pos = effectName.rfind ('\\');
	effectName.erase (0, pos+1);
   pos = effectName.rfind('.');
   if ( pos != effectName.npos )
      effectName.erase(pos, 4);

	// read in the scroll speeds
	stream.read ((char*)&scrollSpeedX, sizeof (int));
	stream.read ((char*)&scrollSpeedY, sizeof (int));

	if (width <= 0 || height <= 0) {
      Console::getInstance().printf("World:create : Invalid layer size!");
		return false;
	}

   if ( !layer.create(name, width, height, effectName) )
   {
      Console::getInstance().printf("World:create : Can not create layer!");
      return false;
   }

	// read in the texture id's and bounds
   int* row = new int[width];
	int len = sizeof(int) * width;

	for (int y = 0; y < height; y++)
   {
		stream.read ((char*)row, len);

		for (int x = 0; x < width; x++)
      {
			layer.setTile(x, y, row[x]);
		}
	}

   return true;
}

bool WorldVersion1Reader::readBounds(std::ifstream& stream)
{
   // now load in the bounds
   int boundcount = 0;
   stream.read ((char*)&boundcount, sizeof (int));

   for ( int i = 0; i < boundcount; i++ )
   {
      int bs[4];
      stream.read ((char*)bs, sizeof(int) * 4);

      getWorld().addBound(Vector(bs[0], bs[1]), Vector(bs[2], bs[3]));
   }

   return true;
}

Layer* WorldVersion1Reader::createLayer()
{
   return LayerFactory::getInstance().create(getWorld().getLayerType());
}

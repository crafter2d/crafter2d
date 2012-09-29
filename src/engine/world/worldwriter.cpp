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
#include "worldwriter.h"
#ifndef JENGINE_INLINE
#  include "worldwriter.inl"
#endif

#include <tinyxml.h>

#include "core/vfs/zipfile.h"
#include "core/streams/bufferedstream.h"

#include "bound.h"
#include "layer.h"
#include "world.h"

DataStream& operator<<(DataStream& in, const Layer& layer);

//////////////////////////////////////////////////////////////////////////
// - Static operations
//////////////////////////////////////////////////////////////////////////

int WorldWriter::getVersion()
{
   return 1;
}

//////////////////////////////////////////////////////////////////////////
// - Construction
//////////////////////////////////////////////////////////////////////////

WorldWriter::WorldWriter():
   mpWorld(NULL)
{
}

WorldWriter::~WorldWriter()
{
}

//////////////////////////////////////////////////////////////////////////
// - Operations
//////////////////////////////////////////////////////////////////////////

bool WorldWriter::write(const World& world, const String& filename)
{
   setWorld(world);

   ZipFile zip;
   if ( zip.create(filename) )
      return writeHeader(zip) && writeSimulator(zip) && writeLayers(zip) && writeBounds(zip);
   else
      return false;
}

bool WorldWriter::writeHeader(ZipFile& zip)
{
   BufferedStream stream(4);

   const int version = getVersion();
   stream.writeInt(version);

   zip.addFile("header", (void*)stream.getData(), stream.getDataSize());

   return true;
}

bool WorldWriter::writeSimulator(ZipFile& zip)
{
   TiXmlDocument doc;
   TiXmlElement* pboundselement = new TiXmlElement("simulator");
   pboundselement->SetAttribute("type", "box2d");
   doc.LinkEndChild(pboundselement);

   std::stringstream stream;
   stream << doc;
   const std::string& data = stream.str();

   zip.addFile("simulator.xml", (void*)data.c_str(), data.length());

   return true;
}

bool WorldWriter::writeLayers(ZipFile& zip)
{
   BufferedStream stream;

   stream << (int)getWorld().getLayerType() << getWorld().getLayerCount();

   for ( int index = 0; index < getWorld().getLayerCount(); ++index )
   {
      const Layer* player = getWorld().getLayer(index);
      stream << *player;
   }

   zip.addFile("data", (void*)stream.getData(), stream.getDataSize());

   return true;
}

bool WorldWriter::writeBounds(ZipFile& zip)
{
   TiXmlDocument doc;
   TiXmlElement* pboundselement = new TiXmlElement("bounds");
   doc.LinkEndChild(pboundselement);

   int count = getWorld().getBoundCount();

   for ( int index = 0; index < count; ++index )
   {
      const Bound& bound = getWorld().getBound(index);
      const Vector& left = bound.getLeft();
      const Vector& right = bound.getRight();

      TiXmlElement* pobject = new TiXmlElement("bound");
      pboundselement->LinkEndChild(pobject);

      pobject->SetDoubleAttribute("left_x", left.x);
      pobject->SetDoubleAttribute("left_y", left.y);
      pobject->SetDoubleAttribute("right_x", right.x);
      pobject->SetDoubleAttribute("right_y", right.y);
   }

   std::stringstream stream;
   stream << doc;
   const std::string& data = stream.str();

   zip.addFile("bounds.xml", (void*)data.c_str(), data.length());

   return true;
}

bool WorldWriter::writeObjects(ZipFile& zip)
{
   TiXmlDocument doc;

   TiXmlElement* pobjects = new TiXmlElement("objects");
   doc.LinkEndChild(pobjects);

   /*
   std::list<SceneObject*>::const_iterator it = getWorld().getChildren().begin();
   for (; it != getWorld().getChildren().end(); it++)
   {
      const SceneObject& object = *(*it);

      TiXmlElement* pobject = new TiXmlElement(object.getRuntimeInfo().getName());

		//object.save(*pobject);

      pobjects->LinkEndChild(pobject);
   }
   */

   std::stringstream stream;
   stream << doc;
   const std::string& data = stream.str();

   zip.addFile("objects.xml", (void*)data.c_str(), data.length());

   return true;
}

//////////////////////////////////////////////////////////////////////////
// - Layer stream functions
//////////////////////////////////////////////////////////////////////////

DataStream& operator<<(DataStream& in, const Layer& layer)
{
   const String&   name       = layer.getName();
   const String&   effectFile = layer.getEffectFile();
   int             width      = layer.getWidth();
   int             height     = layer.getHeight();
   
   in << name << effectFile << width << height;

   for ( int y = 0; y < height; ++y )
   {
      for ( int x = 0; x < width; ++x )
      {
         int tile = layer.getTile(x, y);
         in << tile;
      }
   }

   return in;
}

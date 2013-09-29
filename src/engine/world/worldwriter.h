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
#ifndef WORLDWRITER_H
#define WORLDWRITER_H

#include "engine/engine_base.h"

class DataStream;
class Layer;
class String;
class World;
class ZipFile;

class ENGINE_API WorldWriter
{
public:
   static int getVersion();

   WorldWriter();
   ~WorldWriter();

 // get/set
   const World&   getWorld();
         void     setWorld(const World& world);

 // operations
   bool write(const World& world, const String& filename);

private:
   bool writeHeader(ZipFile& zip);
   bool writeSimulator(ZipFile& zip);
   bool writeLayers(ZipFile& zip);
   bool writeBounds(ZipFile& zip);
   bool writeObjects(ZipFile& zip);

   void writeLayer(DataStream& out, const Layer& layer);

   const World*   mpWorld;
};

#ifdef JENGINE_INLINE
#  include "worldwriter.inl"
#endif

#endif // WORLDWRITER_H

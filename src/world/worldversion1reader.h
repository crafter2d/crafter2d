/***************************************************************************
 *   Copyright (C) 2006 by Jeroen Broekhuizen                              *
 *   jeroen@jengine.homedns.org                                            *
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
 **************************************************************************/
#ifndef WORLDVERSION1READER_H
#define WORLDVERSION1READER_H

#include "abstractworldversionreader.h"

class ifstream;
class Layer;

class WorldVersion1Reader : public AbstractWorldVersionReader
{
public:
   WorldVersion1Reader(World& world);
   virtual ~WorldVersion1Reader();

protected:
 // operations
   virtual bool virRead();

private:
 // operations
   bool     readLayers(std::ifstream& stream);
   bool     readLayer(std::ifstream& stream, Layer& layer);
   bool     readBounds(std::ifstream& stream);

   Layer*   createLayer();
};

#endif // WORLDVERSION1READER_H

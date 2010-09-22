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
#ifndef WORLDVERSION2READER_H
#define WORLDVERSION2READER_H

#include "abstractworldversionreader.h"

class UnzipFile;
class TiXmlDocument;

class WorldVersion2Reader : public AbstractWorldVersionReader
{
public:
   static int getVersion();

   WorldVersion2Reader(World& world);
   virtual ~WorldVersion2Reader();

protected:
   virtual bool virRead();

   bool readHeader(UnzipFile& zip);
   bool readSimulator(UnzipFile& zip);
   bool readLayers(UnzipFile& zip);
   bool readBounds(UnzipFile& zip);
   bool readObjects(UnzipFile& zip);

   bool loadXmlFromZip(UnzipFile& zip, TiXmlDocument& doc, const std::string& filename);
};

#endif // WORLDVERSION2READER_H
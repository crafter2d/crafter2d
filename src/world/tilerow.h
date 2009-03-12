/***************************************************************************
 *   Copyright (C) 2004 by Jeroen Broekhuizen                              *
 *   jeroen@nwnx.homedns.org                                               *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#ifndef TILE_ROW_H_
#define TILE_ROW_H_

class Tile;

/**
@author Jeroen Broekhuizen
\brief Implements one row of tiles. You should not use this class if it can be avoided.

To represent a field the engine uses this class to implement a row of tiles. When implementing your
game try not to use this class unless you have a reason to do so.
*/
class TileRow
{
public:
   TileRow();
   virtual ~TileRow() = 0;

   TileRow& operator=(const TileRow& row);

   int getWidth() const { return _width; }

   bool create(int width);
   void release();

   const Tile& operator[](int index) const;
         Tile& operator[](int index);

protected:
   virtual Tile* createTiles(int width) = 0;

private:
   Tile* _ptiles;
   int   _width;
};

#endif

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
#include "tilerow.h"

#include "../console.h"
#include "../defines.h"

#include "tile.h"

/// \fn TileRow::TileRow()
/// \brief initialized member variables
TileRow::TileRow():
   _ptiles(NULL),
   _width(0)
{
}

/// \fn TileRow::~TileRow()
/// \brief Releases internal member variables
TileRow::~TileRow()
{
	release ();
}

TileRow& TileRow::operator=(const TileRow& row)
{
   int size = MIN(getWidth(), row.getWidth());

   for ( int index = 0; index < size; ++index )
   {
      operator[](index) = row[index];
   }

   return *this;
}

/// \fn TileRow::create(int w)
/// \brief Creates the internal representation of the tile row implemented as a simple array.
/// \param w width of the row
/// \retval true if the tile row is successfully allocated
/// \retval false invalid row size or system out of memory
bool TileRow::create(int width)
{
	// validate the width of the row
	if (width < 0) return false;

   release();

	_width = width;
	_ptiles = createTiles(width);
	if ( _ptiles == NULL )
   {
		Console::getInstance().print("TileRow.Create: out of memory.");
		return false;
	}

	return true;
}

/// \fn TileRow::release()
/// \brief Releases the tile row array
/// \return Nothing
void TileRow::release()
{
	if ( _ptiles != NULL )
   {
		delete[] _ptiles;
		_ptiles = NULL;
	}
}

Tile* TileRow::createTiles(int width)
{
   PURE_VIRTUAL
   return NULL;
}


/// \fn TileRow::operator[](int idx)
/// \brief Use this function to get the tile on index idx. Asserts on an invalid index.
/// \param idx the index of the tile you want
/// \return Reference to the requested tile
Tile& TileRow::operator[](int index)
{
	ASSERT_MSG(index >= 0 && index < _width, "TileRow.operator[]: Invalid tile index.");
	return _ptiles[index];
}

const Tile& TileRow::operator[](int index) const
{
   ASSERT_MSG(index >= 0 && index < _width, "TileRow.operator[]: Invalid tile index.");
	return _ptiles[index];
}

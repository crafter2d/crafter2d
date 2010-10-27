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
#include "core/defines.h"

/*!
    \fn Tile::setTextureId(int tex)
	 \brief Set the texture id of this tile. Texture id is the subtexture from the tileset texture for a layer.
	 \param[in] tex the texture id
	 \return Nothing
 */
void Tile::setTextureId (int tex)
{
	texture = tex;
}

/*!
    \fn Tile::getTextureId()
	 \brief Returns the texture id for this tile.
	 \retval -1 if texture was not set
	 \retval n where n is a number between 0 and the maximum textures in the tileset.
 */
int Tile::getTextureId() const
{
	return texture;
}

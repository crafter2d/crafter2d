/***************************************************************************
 *   Copyright (C) 2013 by Jeroen Broekhuizen                              *
 *   jengine.sse@live.nl                                                   *
 *                                                                         *
 *   This library is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU Lesser General Public License as        *
 *   published by the Free Software Foundation; either version 2.1 of the  *
 *   License, or (at your option) any later version.                       *
 *                                                                         *
 *   This library is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU Lesser General Public License for more details.                   *
 *                                                                         *
 *   You should have received a copy of the GNU Library General Public     *
 *   License along with this program; if not, write to the                 *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#include "spritebatch.h"

#include <algorithm>

#include "sprite.h"

namespace Graphics
{

const Sprite& SpriteBatch::operator[](int index) const
{
   ASSERT(index < size());
   return *mSprites[index];
}

// - Query

int SpriteBatch::size() const
{
   return mSprites.size();
}

// - Operations

void SpriteBatch::add(const Sprite& sprite)
{
   mSprites.push_back(&sprite);
}

void SpriteBatch::clear()
{
   mSprites.clear();
}

void SpriteBatch::sort()
{
   std::sort(mSprites.begin(), mSprites.end(), [](const Sprite* pleft, const Sprite* pright)
   {
      return &pleft->getTexture() < &pright->getTexture();
   });
}

} // namespace Graphics

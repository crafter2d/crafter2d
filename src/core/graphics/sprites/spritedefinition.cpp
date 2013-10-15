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
#include "spritedefinition.h"

namespace Graphics
{

SpriteDefinition::SpriteDefinition():
   mSize(),
   mTexture(),
   mpAnimator(NULL)
{
}

// - Get/set

const Size& SpriteDefinition::getSize() const
{
   return mSize;
}
   
void SpriteDefinition::setSize(const Size& size)
{
   mSize = size;
}

const Texture& SpriteDefinition::getTexture() const
{
   ASSERT(mTexture.isValid());
   return *mTexture;
}
   
void SpriteDefinition::setTexture(const TexturePtr& texture)
{
   mTexture = texture;
}

bool SpriteDefinition::hasSpriteAnimator() const
{
   return mpAnimator != NULL;
}

const Animator& SpriteDefinition::getSpriteAnimator() const
{
   ASSERT_PTR(mpAnimator);
   return *mpAnimator;
}

void SpriteDefinition::setSpriteAnimator(Animator* panimator)
{
   mpAnimator = panimator;
}

} // namespace Graphics
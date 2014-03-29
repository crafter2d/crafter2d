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
#ifndef SPRITE_DEFINITION_H
#define SPRITE_DEFINITION_H

#include "core/string/string.h"
#include "core/math/size.h"
#include "core/core_base.h"

namespace Graphics
{
   class Animator;
   class Texture;

   class CORE_API SpriteDefinition
   {
   public:
      SpriteDefinition();

    // get/set
      const Size& getSize() const;
      void        setSize(const Size& size);

      const Texture& getTexture() const;
      void           setTexture(Texture* ptexture);

      bool            hasSpriteAnimator() const;
      const Animator& getSpriteAnimator() const;
      void            setSpriteAnimator(Animator* panimator);

   private:

    // data
      Size        mSize;
      Texture*    mpTexture;
      Animator*   mpAnimator;
   };
}

#endif // SPRITE_DEFINITION_H

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
#ifndef SPRITE_H
#define SPRITE_H

#include "core/math/size.h"
#include "core/math/xform.h"
#include "core/resource/resourcemanager.h"
#include "core/graphics/texture.h"
#include "core/graphics/texturecoordinate.h"
#include "core/graphics/animation/animationstate.h"

namespace Graphics
{
   class SpriteDefinition;
   class Texture;

   class CORE_API Sprite
   {
   public:
      explicit Sprite(const SpriteDefinition& definition);

    // get/set
      const Size&              getSize() const;
      void                     setSize(const Size& size);

      const Size&              getHalfSize() const;

      const Texture&           getTexture() const;

      const TextureCoordinate& getTextureCoordinate() const;
      void                     setTextureCoordinate(const TextureCoordinate& coordinate);

      const XForm&             getTransform() const;
      void                     setTransform(const XForm& xform);

    // operations
      bool initialize(Device& device);
      void update(float delta);
      void setAnimation(int index);

   private:

    // data
      const SpriteDefinition& mDefinition;

      AnimationState    mAnimState;
      TexturePtr        mTexture;
      TextureCoordinate mTexCoordinate;
      XForm             mTransform;
      Size              mHalfSize;
   };
}

#endif // SPRITE_H

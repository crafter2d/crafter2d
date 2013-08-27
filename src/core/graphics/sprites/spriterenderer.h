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
#ifndef SPRITE_RENDERER_H
#define SPRITE_RENDERER_H

#include <vector>

#include "core/graphics/effect.h"
#include "core/math/vertex.h"
#include "core/defines.h"

#include "spritebatch.h"

namespace Graphics
{
   class Device;
   class RenderContext;
   class UniformBuffer;
   class VertexBuffer;
   class IndexBuffer;
   class Sprite;

   class CORE_API SpriteRenderer
   {
   public:
      SpriteRenderer();

      bool create(Device& device);

      void beginDraw(RenderContext& context);
      void endDraw(RenderContext& context);

      void draw(const Sprite& sprite);

   private:   
      struct PTVertex
      {
         Vertex pos;
         Vertex tex;
      };

    // rendering
      void renderSprite(const Sprite& sprite, PTVertex* pbuffer);

    // data
      SpriteBatch    mSprites;
      Effect         mEffect;
      UniformBuffer* mpUB;
      VertexBuffer*  mpVB;
      IndexBuffer*   mpIB;
   };
}

#endif // SPRITE_RENDERER_H

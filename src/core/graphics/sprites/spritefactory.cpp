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
#include "spritefactory.h"

#include "core/smartptr/autoptr.h"
#include "core/graphics/device.h"

#include "sprite.h"
#include "spritedefinition.h"

namespace Graphics
{

   Sprite* SpriteFactory::create(Graphics::Device& device, const SpriteDefinition& definition)
   {
      AutoPtr<Sprite> result = new Sprite(definition);
      if ( result->initialize(device) )
         return result.release();

      return NULL;

      /*
      TexturePtr texture = ResourceManager::getInstance().getTexture(device, definition.getTextureName());
      result->setTexture(texture);
      result->setSize(definition.getSize());
      result->setAnimator(definition.getSpriteAnimator());

      return result.release();
      */
   }

} // namespace Graphics

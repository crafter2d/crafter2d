/***************************************************************************
 *   Copyright (C) 2006 by Jeroen Broekhuizen                              *
 *   jeroen@jengine.homedns.org                                            *
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
#include "../world/layer.h"
#include "../world/world.h"

#include "../texture.h"
#include "../resources.h"

#include "guidesigner.h"
#include "guiimagelistbox.h"
#ifndef JENGINE_INLINE
#  include "guiimagelistbox.inl"
#endif

REGISTER_DESIGNER(GuiImageListBox, GuiTileImageListBoxId, "Tile Image List" , 50, 50, 2440)

GuiImageListBox::GuiImageListBox() :
   GuiListBox(),
   MPWorld(NULL),
   _activeLayer(-1)
{
}

//////////////////////////////////////////////////////////////////////////
// - Painting interface
//////////////////////////////////////////////////////////////////////////

void GuiImageListBox::drawItem(int index, int posX, int posY, const GuiGraphics& graphics)
{
   if ( hasActiveLayer() )
   {
      const Layer* player = world().getLayer(getActiveLayer());
      const Vector& texcoords = player->tilecoordinates(index+1);
      const Texture& texture = player->getEffect().resolveTexture("diffuseMap");

      GuiRect src(texcoords.x, texcoords.x + player->getTexTileWidth(), texcoords.y, texcoords.y + player->getTexTileHeight());
      GuiRect dest(posX, posX + player->tilewidth(), posY, posY + player->tileheight());

      graphics.setColor(1,1,1);
      graphics.drawImage(texture, src, dest);
   }
}

int GuiImageListBox::measureItem(int index)
{
   if ( hasActiveLayer() )
      return world().getLayer(getActiveLayer())->tileheight() + 5;
   else
      return -1;
}

//////////////////////////////////////////////////////////////////////////
// - Get/set interface
//////////////////////////////////////////////////////////////////////////

void GuiImageListBox::world(const World* pworld)
{
   MPWorld = pworld;

   setActiveLayer(pworld->getLayerCount() > 0 ? 0 : -1);
}

void GuiImageListBox::setActiveLayer(int activeLayer)
{
   if ( getActiveLayer() != activeLayer )
   {
      _activeLayer = activeLayer;
      removeAll();

      if ( _activeLayer >= 0 )
      {
         int count = world().getLayer(getActiveLayer())->tilecount();
         for ( int idx = 0; idx < count; ++idx )
         {
            addString(" ");
         }
      }
   }
}


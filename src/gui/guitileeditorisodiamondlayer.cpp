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
#include "guitileeditorisodiamondlayer.h"

#include "../world/isodiamondlayer.h"

GuiTileEditorIsoDiamondLayer::GuiTileEditorIsoDiamondLayer(Layer& layer):
   GuiTileEditorLayer(layer)
{
}

GuiTileEditorIsoDiamondLayer::~GuiTileEditorIsoDiamondLayer()
{
}

GuiPoint GuiTileEditorIsoDiamondLayer::pointToTile(const GuiPoint& point)
{
   const IsoDiamondLayer& layer = static_cast<IsoDiamondLayer&>(getLayer());
   GuiPoint p(point.x - layer.getXOffset(), point.y);

   int y = (2 * p.y - p.x) / 2;
   int x = p.x + y;

   float xtile = static_cast<float>(x) / layer.tileheight() + 0.5;
   float ytile = static_cast<float>(y) / layer.tileheight() + 0.5;

   return GuiPoint(xtile, ytile);
}

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
#ifndef TOPDOWN_LAYER_H_
#define TOPDOWN_LAYER_H_

#include "layer.h"

class TopDownLayer : public Layer
{
public:
           TopDownLayer();
   virtual ~TopDownLayer();

   virtual LayerType getType() const;

 // creation
   virtual bool initialize(Graphics::Device& device);

 // operations
   virtual void draw(Graphics::RenderContext& context);
   virtual void drawHighlight(const Vector& point);

 // coordinate conversion
   virtual Point pointToTile(const Point& point);

 // notifications
   virtual void onViewportChanged(const Graphics::Viewport& viewport);

protected:
   virtual TileRow*  createTileRows(int width, int height);
};

#endif
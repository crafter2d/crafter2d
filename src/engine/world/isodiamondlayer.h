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
#ifndef ISO_DIAMOND_LAYER_H_
#define ISO_DIAMOND_LAYER_H_

#include "layer.h"

class IsoDiamondLayer : public Layer
{
public:
           IsoDiamondLayer();
   virtual ~IsoDiamondLayer();

 // creations
   virtual bool initialize(Graphics::Device& device);

 // get/set interface
   virtual LayerType  getType() const;

   float getXOffset() const;

 // operations
   virtual void draw(Graphics::RenderContext& context);
   virtual void drawFront(Graphics::RenderContext& context);

 // coordinate conversion
   virtual Point  pointToTile(const Point& point);

 // notifications
   virtual void onViewportChanged(const Graphics::Viewport& viewport);
   
private:

 // drawing
   void drawTile(float** pdata, LayerLevel level, int x, int y, int xpos, int ypos);

 // data
   float _halfTileWidth;
   float _halfTileHeight;
   float _xstart;
};

#endif

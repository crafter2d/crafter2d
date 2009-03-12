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
#ifndef ISO_DIAMOND_LAYER_H_
#define ISO_DIAMOND_LAYER_H_

#include "layer.h"

class IsoDiamondLayer : public Layer
{
public:
           IsoDiamondLayer();
   virtual ~IsoDiamondLayer();

 // get/set interface
   virtual LayerType  getType() const;

   float getXOffset() const;

 // operations
   virtual bool prepare(int screenWidth, int screenHeight);
   virtual void draw();
   virtual void drawHighlight(const Vector& point);

   virtual Point  pointToTile(const Point& point);

protected:
   virtual TileRow*  createTileRows(int width, int height);

private:
   float _halfTileWidth;
   float _halfTileHeight;
   float _xstart;
};

#endif

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

#include "core/math/vertex.h"

#include "layer.h"

class TopDownLayer : public Layer
{
public:
           TopDownLayer();
   virtual ~TopDownLayer();

   virtual LayerType getType() const;

 // creation
   virtual bool initialize(Graphics::Device& device) override;

 // operations
   virtual void draw(Graphics::RenderContext& context) override;
   virtual void drawFront(Graphics::RenderContext& context) override;
   
 // coordinate conversion
   virtual c2d::Point pointToTile(const c2d::Point& point) override;

 // notifications
   virtual void onViewportChanged(Graphics::RenderContext& context, const Graphics::Viewport& viewport) override;

private:

   struct pv
   {
      Vertex pos;
      Vertex tex;
   };

 // updating
   void updateBuffers(Graphics::RenderContext& context);
   void updateTile(pv** pdata, int& verts, LayerLevel level, int x, int y, float xpos, float ypos);
};

#endif
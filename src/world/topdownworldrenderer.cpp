/***************************************************************************
 *   Copyright (C) 2010 by Jeroen Broekhuizen                              *
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
#include "topdownworldrenderer.h"

#include <GL/GLee.h>

#include "world.h"

TopDownWorldRenderer::TopDownWorldRenderer(World& world):
   WorldRenderer(world)
{
}

TopDownWorldRenderer::~TopDownWorldRenderer()
{
}

// - Rendering

void TopDownWorldRenderer::render(float delta)
{
   // draw the scenegraph
   glColor4f(1,1,1,1);
 
   getWorld().draw(true);

   // see if there is any creature under the mouse
   //pick(tick, graph);

   // reset the matrix
   glLoadIdentity();



   /*
   IsometricRenderVisitor visitor;
   visitor.traverse(graph.getRoot());

   SortedObjectList& list = visitor.getObjects();
   for ( ListIterator<Object> it(list); it.isValid(); ++it )
   {
      Object& object = it.item();
      object.draw();
   }
   */
}
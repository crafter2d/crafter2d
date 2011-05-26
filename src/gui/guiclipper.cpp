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
#include "guiclipper.h"

#include <GL/GLee.h>

GuiClipper::GuiClipper(const Rect& rect):
   scissor()
{
   glGetIntegerv(GL_SCISSOR_BOX, scissor);
   setScissorRect(rect);
}

GuiClipper::~GuiClipper()
{
   glScissor(scissor[0], scissor[1], scissor[2], scissor[3]);
}

void GuiClipper::setScissorRect(const Rect& rect)
{
   //int screenheight = Game::getInstance().getScreenHeight();
   int screenheight = 600;

   // determine the correct clipping rectangle
   int x = rect.getLeft();
   if ( x < scissor[0] )
      x = scissor[0];
   int y = screenheight - rect.getBottom();
   if ( y < scissor[1] )
      y = scissor[1];

   int w = rect.getWidth();
   if ( x + w > scissor[0] + scissor[2] )
      w = scissor[0] + scissor[2] - x;
   int h = rect.getHeight();
   if ( y + h > scissor[1] + scissor[3] )
      h = scissor[1] + scissor[3] - y;

   // set the scissor box
   glScissor(x-1, y-1, w+1, h+1);
   glEnable(GL_SCISSOR_TEST);
}

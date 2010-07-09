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
#include "guicontrol.h"
#ifndef JENGINE_INLINE
#  include "guicontrol.inl"
#endif

#include "../game.h"
#include "../script.h"
#include "../scriptmanager.h"

#include "guiclipper.h"
#include "guiwindow.h"

GuiControl::GuiControl():
   GuiWnd(),
   _background()
{
}

GuiControl::~GuiControl()
{
}

void GuiControl::onCreate(const GuiRect& rect, const char* caption, GuiStyle style, GuiWnd* parent)
{
   GuiWnd::onCreate(rect, caption, style, parent);

   GuiCanvas& canvas = Game::getInstance().getCanvas();
   _background = canvas.getDefaultColor(GuiCanvas::GuiWindowColor);
}

void GuiControl::onRender (Uint32 tick, const GuiGraphics& graphics)
{
   // make sure we keep drawing inside the window
   GuiRect windowRect(m_frameRect);
   clientToWindow(windowRect);
   GuiClipper clipper(windowRect);

   if ( getBackground() )
   {
      if (background.isValid())
         graphics.drawImage(background, m_frameRect);
      else 
      {
         graphics.setColor(_background);
         graphics.drawRect(m_frameRect);
      }
   }

   if ( getBorder() )
   {
      graphics.setColor(graphics.canvas().getDefaultColor(GuiCanvas::GuiBorderColor));
      graphics.drawWireRect(m_frameRect);

      glScissor(windowRect.left()+1,600-windowRect.bottom()+1, windowRect.getWidth()-2, windowRect.getHeight()-2);
   }
   else
      glScissor(windowRect.left(),600-windowRect.bottom(), windowRect.getWidth()+1, windowRect.getHeight()+1);

   // render the control itself
   glColor3f(1,1,1);
   paint(tick, graphics);
   glColor3f(1,1,1);

   // draw the children
   renderChildren(tick, graphics);
}

int GuiControl::getTypeId() const
{
   return 0;
}

//////////////////////////////////////////////////////////////////////////
// - Searching
//////////////////////////////////////////////////////////////////////////

float GuiControl::getPreferredWidth() const
{
   return 0;
}

float GuiControl::getPreferredHeight() const
{
   return 0;
}

bool GuiControl::growHorizontally() const
{
   return false;
}

bool GuiControl::growVertically() const
{
   return false;
}

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
#include "guiwindow.h"
#ifndef JENGINE_INLINE
#  include "guiwindow.inl"
#endif

#include "../game.h"

#include "guiclipper.h"
#include "guigraphics.h"
#include "guicontrol.h"

GuiWindow::GuiWindow():
   GuiWnd()
{
}

//////////////////////////////////////////////////////////////////////////
// - Construction
//////////////////////////////////////////////////////////////////////////

void GuiWindow::destroy()
{
   GuiWnd::destroy();
}

//////////////////////////////////////////////////////////////////////////
// - Painting
//////////////////////////////////////////////////////////////////////////

void GuiWindow::onRender(Uint32 tick, const GuiGraphics& graphics)
{
   GuiClipper clipper(m_frameRect);
   GuiWnd::onRender(tick, graphics);
}

//////////////////////////////////////////////////////////////////////////
// - Interface methods
//////////////////////////////////////////////////////////////////////////

bool GuiWindow::isBoundaryWnd() const
{
   return true;
}

GuiWnd* GuiWindow::getBoundaryParent()
{
   return this;
}

void GuiWindow::setActive(bool active)
{
   if ( active )
   {
      Game::getInstance().getCanvas().setActiveWnd(this);
   }
}

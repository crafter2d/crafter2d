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

#include "guiheadercontrol.h"
#ifndef JENGINE_INLINE
#  include "guiheadercontrol.inl"
#endif

#include <algorithm>

#include "guiheadercolumn.h"

GuiHeaderCtrl::GuiHeaderCtrl():
   GuiControl(),
   mMouseListener(*this)
{
}

GuiHeaderCtrl::~GuiHeaderCtrl()
{
}

//////////////////////////////////////////////////////////////////////////
// - Construction
//////////////////////////////////////////////////////////////////////////

bool GuiHeaderCtrl::create (GuiId id, GuiStyle style, GuiWnd* parent)
{
   if ( parent == NULL )
      return false;

   const GuiRect& parentRect = parent->getWindowRect();
   GuiRect rect(1, parentRect.getWidth()-16, 1, height());

   GuiWnd::create(id, rect, "", style, parent);

   addMouseListener(mMouseListener);

   return true;
}

//////////////////////////////////////////////////////////////////////////
// - Painting
//////////////////////////////////////////////////////////////////////////

class GuiColumnDrawer
{
   const GuiGraphics& graphics;

public:
   GuiColumnDrawer(const GuiGraphics& graphics):
      graphics(graphics)
   {
   }

   void operator() (GuiHeaderColumn* column)
   {
      column->paint(graphics);
   }
};

void GuiHeaderCtrl::paint(Uint32 tick, const GuiGraphics& graphics)
{
   std::for_each( columns.begin(), columns.end(), GuiColumnDrawer(graphics));
}

//////////////////////////////////////////////////////////////////////////
// - Interface methods
//////////////////////////////////////////////////////////////////////////

int GuiHeaderCtrl::addColumn(int width)
{
   GuiHeaderColumn* pcolumn = new GuiHeaderColumn(this, calculateNewLeftSize(), width);
   columns.push_back(pcolumn);
   return columns.size() - 1;
}

void GuiHeaderCtrl::removeColumn(int index)
{
}

//////////////////////////////////////////////////////////////////////////
// - Helper functions
//////////////////////////////////////////////////////////////////////////

int GuiHeaderCtrl::calculateNewLeftSize()
{
   int x = 0;
   Columns::const_iterator it = columns.begin();
   for (; it != columns.end(); ++it)
   {
      const GuiHeaderColumn& column = *(*it);
      x += column.width();
   }
   return x;
}

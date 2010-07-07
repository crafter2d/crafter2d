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
#include "../defines.h"

#include "guidesignselection.h"
#include "guidesigndecorator.h"
#include "guidesignwnd.h"

GuiDesignSelection::GuiDesignSelection():
   GuiControl(),
   mMouseListener(*this),
   mBorders(eNone)
{
}

GuiDesignSelection::~GuiDesignSelection()
{
}

// - Creation

void GuiDesignSelection::onCreate(const GuiRect& rect, const char* caption, GuiStyle style, GuiWnd* parent)
{
   GuiControl::onCreate(rect, caption, style, parent);

   addMouseListener(mMouseListener);
}

//////////////////////////////////////////////////////////////////////////
// - Painting interface
//////////////////////////////////////////////////////////////////////////

void GuiDesignSelection::onRender(Uint32 tick, const GuiGraphics& graphics)
{
   GLboolean enabled = glIsEnabled(GL_SCISSOR_TEST);
   glDisable(GL_SCISSOR_TEST);
   const GuiRect& rect = getWindowRect();

   graphics.setColor(1,0,0);
   graphics.drawRect(GuiRect(rect.left()-2, rect.left()+3, rect.top()-2, rect.top()+3));
   graphics.drawRect(GuiRect(rect.left()-2, rect.left()+3, rect.bottom()-3, rect.bottom()+2));
   graphics.drawRect(GuiRect(rect.right()-3, rect.right()+2, rect.top()-2, rect.top()+3));
   graphics.drawRect(GuiRect(rect.right()-3, rect.right()+2, rect.bottom()-3, rect.bottom()+2));
   graphics.setColor(1,1,1);

   if ( enabled )
      glEnable(GL_SCISSOR_TEST);
}

GuiWnd* GuiDesignSelection::hitTest(const GuiPoint& point)
{
   // can not be selected
   return NULL;
}

//////////////////////////////////////////////////////////////////////////
// - Operations
//////////////////////////////////////////////////////////////////////////

GuiDesignWnd* GuiDesignSelection::getDesignWnd() const
{
   GuiWnd* pwnd;
   GuiDesignDecorator* pdecorator = dynamic_cast<GuiDesignDecorator*>(getParent());
   if ( pdecorator )
      pwnd = pdecorator->getParent();
   else
      pwnd = getParent();

   return dynamic_cast<GuiDesignWnd*>(pwnd);
}

void GuiDesignSelection::resize(const GuiPoint& rel, int borders)
{
   int modifier = 1;
   if ( IS_SET(borders, eLeft) )
   {
      getParent()->moveWindow(rel.x, 0);
      modifier = -1;
   }
   if ( IS_SET(borders, eTop) )
   {
      getParent()->moveWindow(0, rel.y);
      modifier = -1;
   }

   int width = getWindowRect().getWidth();
   if ( IS_SET(borders, eLeft) || IS_SET(borders, eRight) )
   {
      width += (rel.x * modifier);
   }

   int height = getWindowRect().getHeight();
   if ( IS_SET(borders, eTop) || IS_SET(borders, eBottom) )
   {
      height += (rel.y * modifier);
   }

   getParent()->resizeWindow(width, height);
}

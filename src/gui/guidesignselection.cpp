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
#include "../defines.h"

#include "guidesignselection.h"
#include "guidesigndecorator.h"
#include "guidesignwnd.h"

GuiDesignSelection::GuiDesignSelection()
 : GuiControl(),
   _border(ENone)
{
}

GuiDesignSelection::~GuiDesignSelection()
{
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
// - Input interface
//////////////////////////////////////////////////////////////////////////

int GuiDesignSelection::onLButtonDown(const GuiPoint& point, int flags)
{
   _border = determineBorder(point);
   return _border != ENone ? JENGINE_MSG_HANDLED : JENGINE_MSG_UNHANDLED;
}

int GuiDesignSelection::onLButtonUp(const GuiPoint& point, int flag)
{
   if ( _border != ENone )
   {
      _border = ENone;

      return JENGINE_MSG_HANDLED;
   }
   else
      return JENGINE_MSG_UNHANDLED;
}

void GuiDesignSelection::onMouseMove(const GuiPoint& point, const GuiPoint& rel, int flag)
{
   if ( _border != ENone )
   {
      GuiDesignDecorator* pdecorator = dynamic_cast<GuiDesignDecorator*>(getParent());
      if ( pdecorator )
      {
         dynamic_cast<GuiDesignWnd*>(pdecorator->getParent())->resizeSelected(rel, _border);
      }
      else
      {
         resize(rel, _border);
      }
   }
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

int GuiDesignSelection::determineBorder(const GuiPoint& point) const
{
   const GuiRect rect(getWindowRect());
   int sides = ENone;

   if ( point.x == rect.left() || point.x - rect.left() <= 2 )
   {
      sides = ELeft;
   }
   else if ( point.x == rect.right() || abs(rect.right() - point.x)  <= 2 )
   {
      sides = ERight;
   }
   
   if ( point.y == rect.top() || point.y - rect.top() <= 2 )
   {
      sides |= ETop;
   }
   else if ( point.y == rect.bottom() || abs(rect.bottom() - point.y) <= 2 )
   {
      sides |= EBottom;
   }
   
   return sides;
}

void GuiDesignSelection::resize(const GuiPoint& rel, int borders)
{
   int modifier = 1;
   if ( borders & ELeft )
   {
      getParent()->moveWindow(rel.x, 0);
      modifier = -1;
   }
   if ( borders & ETop )
   {
      getParent()->moveWindow(0, rel.y);
      modifier = -1;
   }

   int width = getWindowRect().getWidth();
   if ( borders & ELeft || borders & ERight )
   {
      width += (rel.x * modifier);
   }

   int height = getWindowRect().getHeight();
   if ( borders & ETop || borders & EBottom )
   {
      height += (rel.y * modifier);
   }

   getParent()->resizeWindow(width, height);
}


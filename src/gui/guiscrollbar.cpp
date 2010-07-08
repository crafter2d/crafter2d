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
#include "guiscrollbar.h"
#ifndef JENGINE_INLINE
#  include "guiscrollbar.inl"
#endif

#include "gui/button/guibutton.h"

GuiScrollBar::GuiScrollBar(): 
   mpTop(NULL),
   mpBottom(NULL),
   mpScroller(NULL),
   scrollRange(0),
   mScrollPage(8),
   scrollPos(0),
   scrollPage(0),
   moveSpeed(0),
   mMouseListener(*this)
{
}

EVENT_MAP_BEGIN(GuiScrollBar)
   ON_BTN_CLICK(1,onScrollUp)
   ON_BTN_CLICK(2,onScrollDown)
EVENT_MAP_END()

void GuiScrollBar::onCreate(const GuiRect &rect, const char* caption, GuiStyle style, GuiWnd* parent)
{
   GuiWnd::onCreate(rect, caption, style, parent);

   float w = rect.getWidth();
   float h = rect.getHeight();

   // we don't have a border
   style &= ~GUI_BORDER;

   mpTop = new GuiButton();
   mpBottom = new GuiButton();
   mpScroller = new GuiButton();

   if ( isHorizontal() )
   {
      mpTop->create(1, GuiRect(0,15,0,h), "", GUI_VISIBLE|GUI_BACKGROUND, this);
      mpBottom->create(2, GuiRect(w-15,w,0,h), "", GUI_VISIBLE|GUI_BACKGROUND, this);
      mpScroller->create(3, GuiRect(17,23,0,h), "", GUI_VISIBLE|GUI_BACKGROUND, this);
   }
   else
   {
      mpTop->create(1, GuiRect(0,w,0,15), "", GUI_VISIBLE|GUI_BACKGROUND, this);
      mpBottom->create(2, GuiRect(0,w,h-15,h), "", GUI_VISIBLE|GUI_BACKGROUND, this);
      mpScroller->create(3, GuiRect(0,w,17,23), "", GUI_VISIBLE|GUI_BACKGROUND, this);
   }

   addMouseListener(mMouseListener);
}

void GuiScrollBar::destroy()
{
   GuiWnd::destroy();

   mpTop = mpBottom = mpScroller = NULL;
}

// - Rendering

void GuiScrollBar::paint(Uint32 tick, const GuiGraphics& graphics)
{
   graphics.setColor(0.37f, 0.639f, 0.674f);
   graphics.drawRect(m_frameRect);
   graphics.setColor(1,1,1);
}

// - Operations

void GuiScrollBar::setScrollRange(int range)
{
   scrollRange = range;
   if ( scrollRange != 0 )
   {
      // resize the scroll button
      float height = (isHorizontal() ? m_frameRect.getWidth() : m_frameRect.getHeight()) - 34 - 6;

      // determine how much the button will move per tick
      moveSpeed = static_cast<int>(height / range);

      setScrollPosition(scrollPos);
   }
}

void GuiScrollBar::setScrollPosition(int pos)
{
   scrollPos = pos;
   if (scrollPos > scrollRange)
      scrollPos = scrollRange;
   else if ( scrollPos < 0 )
      scrollPos = 0;

   int linepos = 17 + scrollPos*moveSpeed;

   if ( isHorizontal() )
      mpScroller->setWindowPos(linepos, 0);
   else
      mpScroller->setWindowPos(0, linepos);

   parent->sendMessage(m_id, GuiScrollPosChangedEvent, scrollPos);
}

void GuiScrollBar::scroll(ScrollType type, int amount)
{
   int newpos = scrollPos;

   if ( type == eLine )
   {
      newpos = scrollPos + amount;
   }
   else if ( type == ePage )
   {
      newpos = scrollPos + amount * mScrollPage;
   }

   setScrollPosition(newpos);
}

void GuiScrollBar::moveScroller(int pos)
{
   if ( isHorizontal() )
      mpScroller->moveWindow(pos, 0);
   else
      mpScroller->moveWindow(0, pos);
}

bool GuiScrollBar::isUpperPart(const Point& point)
{
   const GuiRect& rect = mpScroller->getWindowRect();
   if ( isHorizontal() )
      return point.x() > 16 && point.x() < rect.left();
   else
      return point.y() > 16 && point.y() < rect.top();
}

bool GuiScrollBar::isLowerPart(const Point& point)
{
   const GuiRect& rect = mpScroller->getWindowRect();
   if ( isHorizontal() )
      return point.x() > rect.right() && point.x() < m_frameRect.getWidth();
   else
      return point.y() > rect.bottom() && point.y() < m_frameRect.getHeight();
}

// - Callbacks

void GuiScrollBar::onScrollUp()
{
   if (scrollPos > 0)
   {
      moveScroller(-moveSpeed);
      scrollPos--;

      parent->sendMessage(m_id, GuiScrollUpEvent, scrollPos);
   }
}

void GuiScrollBar::onScrollDown()
{
   if (scrollPos < scrollRange)
   {
      moveScroller(moveSpeed);
      scrollPos++;

      parent->sendMessage(m_id, GuiScrollDownEvent, scrollPos);
   }
}

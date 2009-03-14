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

GuiScrollBar::GuiScrollBar(void): 
   top(0),
   bottom(0),
   scroller(0),
   scrollRange(0),
   scrollPos(0),
   scrollPage(0),
   moveSpeed(0)
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

   top = new GuiButton();
   bottom = new GuiButton();
   scroller = new GuiButton();

   if ( isHorizontal() )
   {
      top->create(1, GuiRect(0,15,0,h), "", GUI_VISIBLE|GUI_BACKGROUND, this);
      bottom->create(2, GuiRect(w-15,w,0,h), "", GUI_VISIBLE|GUI_BACKGROUND, this);
      scroller->create(3, GuiRect(17,23,0,h), "", GUI_VISIBLE|GUI_BACKGROUND, this);
   }
   else
   {
      top->create(1, GuiRect(0,w,0,15), "", GUI_VISIBLE|GUI_BACKGROUND, this);
      bottom->create(2, GuiRect(0,w,h-15,h), "", GUI_VISIBLE|GUI_BACKGROUND, this);
      scroller->create(3, GuiRect(0,w,17,23), "", GUI_VISIBLE|GUI_BACKGROUND, this);
   }
}

void GuiScrollBar::destroy()
{
   GuiWnd::destroy();

   top = bottom = scroller = 0;
}

void GuiScrollBar::setScrollRange(int range)
{
   scrollRange = range;
   if ( scrollRange != 0 )
   {
      // resize the scroll button
      float height = (isHorizontal() ? m_frameRect.getWidth() : m_frameRect.getHeight()) - 34 - 6;

      // determine how much the button will move per tick
      moveSpeed = static_cast<int>(height / range);

      if ( scrollPos > scrollRange )
         scrollPos = scrollRange;

      setScrollPosition(scrollPos);
   }
}

void GuiScrollBar::setScrollPosition(int pos)
{
   if (pos <= scrollRange)
   {
      scrollPos = pos;

      int linepos = 17 + pos*moveSpeed;

      if ( isHorizontal() )
         scroller->setWindowPos(linepos, 0);
      else
         scroller->setWindowPos(0, linepos);

      parent->sendMessage(m_id, GuiScrollPosChangedEvent, scrollPos);
   }
}

void GuiScrollBar::paint(Uint32 tick, const GuiGraphics& graphics)
{
   graphics.setColor(0.37f, 0.639f, 0.674f);
   graphics.drawRect(m_frameRect);
   graphics.setColor(1,1,1);
}

int GuiScrollBar::onLButtonUp (const GuiPoint& point, int flags)
{
   GuiControl::onLButtonUp(point, flags);

   GuiPoint p(point);
   windowToClient(p);
   if ( top->hitTest(p) || scroller->hitTest(p) || bottom->hitTest(p) )
      return 0;

   if ( isUpperPart(p) )
   {
      // scroll up a page
      int move = (scrollPos > 8 ? 8 : scrollPos);
      if (move > 0)
      {
         moveScroller(-moveSpeed*move);
         scrollPos -= move;
         parent->sendMessage(m_id,GuiScrollUpEvent,scrollPos);
      }
   }
   else if ( isLowerPart(p) )
   {
      // scroll down a page
      int move = ((scrollRange-scrollPos) > 8 ? 8 : (scrollRange-scrollPos));
      if (move > 0)
      {
         moveScroller(moveSpeed*move);
         scrollPos += move;
         parent->sendMessage(m_id,GuiScrollDownEvent,scrollPos);
      }
   }

   return 0;
}

void GuiScrollBar::onScrollUp()
{
   if (scrollPos > 0) {
      moveScroller(-moveSpeed);
      scrollPos--;

      parent->sendMessage(m_id,GuiScrollUpEvent,scrollPos);
   }
}

void GuiScrollBar::onScrollDown()
{
   if (scrollPos < scrollRange) {
      moveScroller(moveSpeed);
      scrollPos++;

      parent->sendMessage(m_id, GuiScrollDownEvent, scrollPos);
   }
}

void GuiScrollBar::moveScroller(int pos)
{
   if ( isHorizontal() )
      scroller->moveWindow(pos, 0);
   else
      scroller->moveWindow(0, pos);
}

bool GuiScrollBar::isUpperPart(const GuiPoint& point)
{
   const GuiRect& rect = scroller->getWindowRect();
   if ( isHorizontal() )
      return point.x > 16 && point.x < rect.left();
   else
      return point.y > 16 && point.y < rect.top();
}

bool GuiScrollBar::isLowerPart(const GuiPoint& point)
{
   const GuiRect& rect = scroller->getWindowRect();
   if ( isHorizontal() )
      return point.x > rect.right() && point.x < m_frameRect.getWidth();
   else
      return point.y > rect.bottom() && point.y < m_frameRect.getHeight();
}

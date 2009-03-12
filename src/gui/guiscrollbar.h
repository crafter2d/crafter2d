/***************************************************************************
 *   Copyright (C) 2004 by Jeroen Broekhuizen                              *
 *   jeroen@nwnx.homedns.org                                               *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#ifndef _GUISCROLLBAR_H_
#define _GUISCROLLBAR_H_

#include "guicontrol.h"
#include "guibutton.h"

class GuiScrollBar : public GuiControl
{
public:
                  GuiScrollBar();

   virtual void   destroy();

   void           setScrollInfo(int pageSize);
   void           setScrollRange(int range);
   void           setScrollPosition(int pos);

   int            getScrollPosition() const;
   int            getScrollRange() const;

   bool           isHorizontal() const;

   virtual void   sendMessage(GuiId, GuiEvent event, int param1);
   virtual int    onLButtonUp (const GuiPoint& point, int flags);

protected:
   virtual void   onCreate (const GuiRect& rect, const char* caption, GuiStyle style, GuiWnd* parent);
   virtual void   paint(Uint32 tick, const GuiGraphics& graphics);

   bool           isUpperPart(const GuiPoint& point);
   bool           isLowerPart(const GuiPoint& point);

   void           moveScroller(int pos);

   void           onScrollUp();
   void           onScrollDown();

   GuiButton   *top;
   GuiButton   *bottom;
   GuiButton   *scroller;

   int      scrollRange;
   int      scrollPos;
   int      scrollPage;
   int      moveSpeed;
};

#ifdef JENGINE_INLINE
#  include "guiscrollbar.inl"
#endif

#endif

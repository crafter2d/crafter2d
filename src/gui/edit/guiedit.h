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

#ifndef _GUIEDIT_H_
#define _GUIEDIT_H_

#include "guicontrol.h"

class GuiScrollBar;

/*! @author Jeroen Broekhuizen
 *
 * class GuiEditBox, implements a basic edit control window with user input.
 */
class GuiEditBox: public GuiControl
{
public:
   static void onCaptionChanged(GuiEditBox* pwnd);
   static void onMultiLineChanged(GuiEditBox* pwnd);
   static void onWordWrapChanged(GuiEditBox* pedit);

   DESIGNER_REGISTRATION(GuiEditBox)
 
	               GuiEditBox();

 // Get/set interface
   void handleCaptionChanged();
   void handleMultiLineChanged();
   void handleWordWrapChanged();

   virtual void   sendMessage(GuiId id, GuiEvent event, int param1);

   virtual int    onLButtonDown(const GuiPoint& point, int flags);
	virtual void   onKeyDown (int which, bool shift, bool ctrl, bool alt);

protected:
   virtual void   onCreate (const GuiRect& rect, const char* caption, GuiStyle style, GuiWnd* parent);
   virtual void   onResize(int width, int height);
   virtual void   onSetFocus(GuiControl* oldCtrl);

   virtual void   paint(Uint32 tick, const GuiGraphics& graphics);

   void  initializeScrollbar();
   void  setVertScrollBarRange();
   void  onScroll(int pos);

   void  paintText();
   int   getLineLength(int index) const;
   int   carretToStringPos(bool useOffset = false) const;
   int   getLineCount() const;
   void  ensureVisible();
   void  skipWhitespace(const char* ptext, bool forward);
   void  skipToWhitespace(const char* ptext, bool forward);
   void  doMove(int direction);

   void  doReturn(bool shift, bool ctrl, bool alt);
   void  doInsert(char character);
   void  doBackspace();
   void  doDelete();
   void  doTab();

   void  moveUp();
   void  moveDown();
   void  moveLeft(bool ctrl);
   void  moveRight(bool ctrl);

   DECLARE_PROPERTIES
   BOOL_PROPERTY(MultiLine)
   BOOL_PROPERTY(WordWrap)

   GuiScrollBar* _pvertscrollbar;

   std::vector< std::string > _lines;

   int      maxChars;
   int      maxLines;
   int      scrollPos;
   int      scrollPosY;

   bool     showCarret;
   GuiPoint carretPos;
   Uint32   lastTick;
};

#endif

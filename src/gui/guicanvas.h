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
#ifndef _GUICANVAS_H_
#define _GUICANVAS_H_

#include "guiwnd.h"
#include "guiwndreference.h"

/**
@author Jeroen Broekhuizen
\brief Canvas for windows and game content.

The GuiCanvas is the important part for the windowing structure. The client
must have a canvas object to be able to display any windows or game content.
*/
class GuiCanvas : public GuiWnd
{
public:
   enum GuiStandardColor {
      GuiWindowColor = 0,
      GuiBorderColor = 1,
      GuiHeaderColor = 2,
      MaxGuiColors = 3
   };

   static bool    isShift();
   static bool    isAlt();
   static bool    isCtrl();

                  GuiCanvas();
   virtual        ~GuiCanvas();

   void           quit();

   void           render(Uint32 tick);

   void           doModal(GuiWnd* wnd);
   void           endModal(int retValue);
   void           pushWindow(GuiWnd* wnd);
   void           popWindow();
   void           popWindow(GuiWnd* wnd);
   void           popAll();

   void           changeDefaultColor(GuiStandardColor color, const GuiColor& newColor);
   void           changeDefaultColor(int color, const GuiColor& newColor);
   GuiColor       getDefaultColor(GuiStandardColor color) const;

   bool           isActiveWnd(const GuiWnd& window) const;
   GuiWnd&        getActiveWnd() const;
   void           setActiveWnd(GuiWnd* wnd);

   virtual void   onMouseMove (const GuiPoint& point, const GuiPoint& rel, int flag);
   virtual void   onMouseWheel (const GuiPoint& point, int direction, int flag);
   virtual int    onLButtonDown (const GuiPoint& point, int flag);
   virtual int    onLButtonUp (const GuiPoint& point, int flag);
   virtual int    onRButtonDown (const GuiPoint& point, int flag);
   virtual void   onKeyDown(int which, bool shift, bool ctrl, bool alt);
   virtual void   onKeyUp (int which);

protected:
   void           setFocus(GuiWnd* pwnd);
   void           findFocusUnderCursor(const GuiPoint& point);

private:
   void           doPopWindow(GuiList::Iterator it);

   static GuiColor   defaultColors[MaxGuiColors];

   GuiGraphics       graphics;
   GuiList           windows;
   GuiList           modal;
   GuiWndReference   activeWnd;
};

#ifdef JENGINE_INLINE
#  include "guicanvas.inl"
#endif

#endif

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
#ifndef _GUIBUTTON_H_
#define _GUIBUTTON_H_

#include <string>
#include "../math/vector.h"

#include "../texture.h"
#include "../resources.h"

#include "guicontrol.h"

/*!
@author Jeroen Broekhuizen
\brief Implementation of a simple button window.

The GuiButton control implements a simple button object which can be displayed on the canvas
or used as child of another window. When the button is clicked a Lua script will be executed
which is set with the GuiButton::setCommand function.
*/
class GuiButton : public GuiControl
{
public:
   DESIGNER_REGISTRATION(GuiButton)

 // statics
   static void onIconChanged(GuiWnd* pwnd);

                  GuiButton();

   void           setCommand (const char* command);

   virtual int    onLButtonDown (const GuiPoint& point, int flag);
   virtual int    onLButtonUp (const GuiPoint& point, int flag);
   virtual void   onKillFocus(GuiControl* newCtrl);
   virtual void   onResize(int width, int height);

   bool           isPressed() const;

   void           click();
   void           loadIcon(const std::string& icon);

protected:
   virtual void   onCreate (const GuiRect& rect, const char* caption, GuiStyle style, GuiWnd* parent);
   virtual void   paint(Uint32 tick, const GuiGraphics& graphics);

   virtual void   initializeEventHandlerDefinitions();

   DECLARE_PROPERTIES
   BOOL_PROPERTY(Default)
   STR_PROPERTY(Icon)

private:
   GuiPoint    pos;
   Uint32      pressTime;
   TexturePtr  _icon;
   GuiRect     _iconRect;
   std::string command;
};

#ifdef JENGINE_INLINE
#  include "guibutton.h"
#endif

#endif

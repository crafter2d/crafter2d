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
#ifndef _GUISCRIPTWND_H_
#define _GUISCRIPTWND_H_

#include <string>
#include "guiwindow.h"

class GuiScriptWnd : public GuiWindow
{
public:
   GuiScriptWnd();

   virtual void   onMouseMove (const GuiPoint& point, const GuiPoint& rel, int flags);
   virtual int    onLButtonUp (const GuiPoint& point, int flags);
   virtual void   onKeyDown (int which, bool shift, bool ctrl, bool alt);

   void           setMouseMoveFnc(const char* fnc = 0);
   void           setLButtonDownFnc(const char* fnc = 0);
   void           setKeyDownFnc(const char* fnc = 0);

protected:
   std::string fncMouseMove;
   std::string fncLButtonDown;
   std::string fncKeyDown;
};

#ifdef JENGINE_INLINE
#  include "guiscriptwnd.inl"
#endif

#endif

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
#ifndef _GUIGAMEWND_H_
#define _GUIGAMEWND_H_

#include <stack>
#include "../actionmap.h"

#include "guiwindow.h"

class SceneGraph;

/// @author Jeroen Broekhuizen
///
/// Displays the actual game content and handles input using an ActionMap
/// which can be set in script.
class GuiGameWnd : public GuiWindow
{
public:
   typedef std::stack<KeyMap*> KeyMaps;

                  GuiGameWnd();

   void           pushMap(KeyMap* map);
   void           popMap();

   virtual int    onLButtonUp(const GuiPoint& point, int flags);

   void           onKeyDown(int which, bool shift, bool ctrl, bool alt);
   void           onKeyUp(int which);
   void           onActivate(bool active, GuiWnd* oldWnd);

   void           pick(Uint32 tick, SceneGraph& graph);

protected:
   virtual void   onRender(Uint32 tick, const GuiGraphics& graphics);

   void  doPick(SceneGraph& graph, bool click);
   void  renderWorld(SceneGraph& graph);
   
   KeyMaps maps;
   float zoom;
};

#ifdef JENGINE_INLINE
#  include "guigamewnd.inl"
#endif

#endif

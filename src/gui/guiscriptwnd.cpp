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
#include "guiscriptwnd.h"
#ifndef JENGINE_INLINE
#  include "guiscriptwnd.inl"
#endif

#include "../script.h"
#include "../scriptmanager.h"

GuiScriptWnd::GuiScriptWnd():
   GuiWindow()
{
}

void GuiScriptWnd::onMouseMove (const GuiPoint& point, const GuiPoint& rel, int flags)
{
   GuiWindow::onMouseMove(point, rel, flags);
   if (!fncMouseMove.empty())
   {
      GuiPoint p(point);
      windowToClient(p);

      Script& script = ScriptManager::getInstance().getTemporaryScript();
      script.prepareCall(fncMouseMove.c_str());
      script.addParam((void*)&p, "GuiPoint");
      script.run(1);
   }
}

int GuiScriptWnd::onLButtonUp (const GuiPoint& point, int flags)
{
   if ( GuiWindow::onLButtonUp(point, flags) != 0 )
   {
      if (!fncLButtonDown.empty())
      {
         GuiPoint p(point);
         windowToClient(p);

         Script& script = ScriptManager::getInstance().getTemporaryScript();
         script.prepareCall(fncLButtonDown.c_str());
         script.addParam((void*)&p, "GuiPoint");
         script.run(1);
      }
   }

   return JENGINE_MSG_HANDLED;
}

void GuiScriptWnd::onKeyDown (int which, bool shift, bool ctrl, bool alt)
{
   GuiWindow::onKeyDown(which, shift, ctrl, alt);
   if (!fncKeyDown.empty())
   {
      Script& script = ScriptManager::getInstance().getTemporaryScript();
      script.prepareCall(fncKeyDown.c_str());
      script.addParam(which);
      script.run(1);
   }
}

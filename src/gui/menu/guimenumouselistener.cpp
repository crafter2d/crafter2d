/***************************************************************************
 *   Copyright (C) 2010 by Jeroen Broekhuizen                              *
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

#include "gui/menu/guimenumouselistener.h"

#include "gui/input/mouseevent.h"

#include "gui/guimenu.h"
#include "gui/guieventhandler.h"
#include "gui/guieventhandlers.h"

#include "scriptmanager.h"
#include "scopedvalue.h"

GuiMenuMouseListener::GuiMenuMouseListener(GuiMenu& menu):
   MouseListener(),
   mMenu(menu)
{
}

// - Notifications

void GuiMenuMouseListener::onMouseClick(const MouseEvent& event)
{
   GuiPoint point = event.getLocation();

   mMenu.windowToClient(point);
   mMenu.selectItem(point);

   if ( mMenu.getSelection() != -1 )
   {
      mMenu.setVisible(false);

      int id = mMenu.getItems()[mMenu.getSelection()].getId();

      GuiEventHandler* phandler = mMenu.getEventHandlers().findByEventType(GuiContextMenuEvent);
      if ( phandler != NULL )
      {
         ScopedValue<bool> value(mMenu._processing, true, false);

         ScriptManager& mgr = ScriptManager::getInstance();
         Script& script = mgr.getTemporaryScript();
         script.prepareCall(phandler->getFunctionName().c_str());
         script.addParam(id);
         script.run(1);
      }
      
      mMenu.getParent()->onCommand(id);
      
      event.consume();
   }
}

void GuiMenuMouseListener::onMouseMotion(const MouseEvent& event)
{
   GuiPoint point = event.getLocation();

   mMenu.windowToClient(point);
   mMenu.selectItem(point);
}

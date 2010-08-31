/***************************************************************************
 *   Copyright (C) 2009 by Jeroen Broekhuizen                              *
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

#include "guilistboxkeylistener.h"

#include "gui/input/keyevent.h"

#include "gui/guilistbox.h"
#include "gui/guieventhandler.h"
#include "gui/guieventhandlers.h"

#include "scriptmanager.h"

GuiListBoxKeyListener::GuiListBoxKeyListener(GuiListBox& listbox):
   KeyListener(),
   mListbox(listbox)
{
}

// - Notifications

void GuiListBoxKeyListener::onKeyReleased(const KeyEvent& event)
{
   GuiEventHandler* phandler = mListbox.getEventHandlers().findByEventType(GuiListKeyPressEvent);
   if ( phandler != NULL )
   {
      ScriptManager& mgr = ScriptManager::getInstance();
      Script& script = mgr.getTemporaryScript();
      script.setSelf(this, &mListbox);
      script.prepareCall(phandler->getFunctionName().c_str());
      script.addParam(event.getKey());
      script.run(1);
   }
}

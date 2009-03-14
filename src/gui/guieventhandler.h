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
#ifndef GUIEVENT_HANDLER_H_
#define GUIEVENT_HANDLER_H_

#include <string>
#include "guidefines.h"

/*!
@author Jeroen Broekhuizen

\brief The actual instance of an event handler. This class is used internally by the engine
only and should not be used. To create new handlers use the GuiEventHandlerDefinition class
as described below.

\section guiEventHandler_intro Introduction
Event handlers are little Lua scripts which are triggered at certain events. For example the
button onclick event handler is triggered in case a player clicks on that button. This script
then should perform any necessary actions, e.g. closing a modal dialog, opening a dialog, etc.

\section guiEventHandler_defs Defining event handlers
JEngine needs to know which event handlers are available for each control type. For each event
handler it also needs to know the name, number of parameters and the handler body (the actual
script). This information is stored in the GuiEventHandlerDefinition class.
Every specialized control that implements event handlers should overload the protected
initializeEventHandlerDefinitions method. In this method the event handlers definitions
for this class will be created. The next code sample demonstrates how the onclick event handler
of the button is implemented.

\code 
void GuiButton::initializeEventHandlerDefinitions()
{
   GuiControl::initializeEventHandlerDefinitions();

   GuiEventHandlerDefinition* pdefinition = new GuiEventHandlerDefinition(GuiButtonClickEvent, "onClick");

   getEventHandlerDefinitions().add(pdefinition);
}
\endcode
 */
class GuiEventHandler
{
public:
   GuiEventHandler(GuiEvent type, const std::string& function);

 // Get/set interface
   const std::string getFunctionName() const;
   GuiEvent getType() const;

   bool   isVisible();
   void   setVisible(bool visible);

private:
   GuiEvent       _type;
   std::string    _functionname;
   bool           _visible;
};

#ifdef JENGINE_INLINE
#  include "guieventhandler.inl"
#endif

#endif

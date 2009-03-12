/***************************************************************************
 * Copyright (C) 2006 Jeroen Broekhuizen                                   *
 * jeroen@jeroen.homedns.org                                               *
 *                                                                         *
 * This library is free software; you can redistribute it and/or           *
 * modify it under the terms of the GNU Lesser General Public              *
 * License as published by the Free Software Foundation; either            * 
 * version 2.1 of the License, or (at your option) any later version.      *
 *                                                                         *
 * This library is distributed in the hope that it will be useful,         *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of          *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU       *
 * Lesser General Public License for more details.                         *
 *                                                                         *
 * You should have received a copy of the GNU Lesser General Public        *
 * License along with this library; if not, write to the                   *
 * Free Software Foundation, Inc.,                                         *
 * 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA            *
 ***************************************************************************/
#ifndef GUIEVENT_HANDLER_DEFINITION_H_
#define GUIEVENT_HANDLER_DEFINITION_H_

#include <string>
#include <vector>

#include "guidefines.h"

class GuiEventHandler;
class GuiWnd;
class TiXmlElement;
class GuiWnd;

class GuiEventHandlerDefinition
{
public:
   typedef std::vector<std::string> Arguments;

   GuiEventHandlerDefinition();
   GuiEventHandlerDefinition(GuiEvent event, const std::string& name);

 // Get/set interface
   GuiEvent           getEvent() const;
   void               setEvent(GuiEvent event);

   const std::string& getName() const;
   void               setName(const std::string& name);

   const std::string& getCode() const;
   void               setCode(const std::string& code);

 // Operations
   void               addArgument(const std::string& argument);
   void               construct(GuiWnd& wnd) const;

 // Storage interface
   bool           needsSave() const;
   TiXmlElement*  toXML() const;
   void           fromXML(const TiXmlElement& element);

private:
   void operator=(const GuiEventHandlerDefinition& definition);

 // Operations
   std::string constructHandlerBody(const std::string& functionname) const;

   Arguments   _arguments;
   std::string _name;
   std::string _code;
   GuiEvent    _event;
};

#endif

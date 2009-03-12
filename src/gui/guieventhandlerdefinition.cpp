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
#include "guieventhandlerdefinition.h"

#include <string>
#include <tinyxml.h>

#include "../console.h"
#include "../defines.h"
#include "../script.h"
#include "../scriptmanager.h"

#include "guiwnd.h"
#include "guieventhandler.h"

GuiEventHandlerDefinition::GuiEventHandlerDefinition():
   _arguments(),
   _name(),
   _code(),
   _event(UndefinedEvent)
{
}

GuiEventHandlerDefinition::GuiEventHandlerDefinition(GuiEvent event, const std::string& name):
   _arguments(),
   _name(name),
   _code(),
   _event(event)
{
}

void GuiEventHandlerDefinition::operator=(const GuiEventHandlerDefinition& definition)
{
}

//////////////////////////////////////////////////////////////////////////
// - Get/set interface
//////////////////////////////////////////////////////////////////////////

GuiEvent GuiEventHandlerDefinition::getEvent() const
{
   return _event;
}

void GuiEventHandlerDefinition::setEvent(GuiEvent event)
{
   _event = event;
}

const std::string& GuiEventHandlerDefinition::getName() const
{
   return _name;
}

void GuiEventHandlerDefinition::setName(const std::string& name)
{
   _name = name;
}

const std::string& GuiEventHandlerDefinition::getCode() const
{
   return _code;
}

void GuiEventHandlerDefinition::setCode(const std::string& code)
{
   _code = code;
}

//////////////////////////////////////////////////////////////////////////
// - Operations
//////////////////////////////////////////////////////////////////////////

void GuiEventHandlerDefinition::addArgument(const std::string& argument)
{
   _arguments.push_back(argument);
}

void GuiEventHandlerDefinition::construct(GuiWnd& wnd) const
{
   if ( !getCode().empty() )
   {
      if ( wnd.getName().empty() )
         Console::getInstance().print("Found window without a name!");
     
      std::string proposal     = wnd.getName() + "_" + getName();
      std::string functionname = ScriptManager::getInstance().generateUniqueFunctionName(proposal);
      std::string handlerBody  = constructHandlerBody(functionname);

      if ( !ScriptManager::getInstance().executeLine(handlerBody.c_str()) )
      {
         std::string error = "Can not load " + getName() + " handler code in Lua.";
         Console::getInstance().print(error.c_str());
      }
      else
      {
         GuiEventHandler* phandler = new GuiEventHandler(getEvent(), functionname);
         wnd.registerEventHandler(phandler);
      }
   }
}

std::string GuiEventHandlerDefinition::constructHandlerBody(const std::string& functionname) const
{
   std::string code = "function " + functionname + "(";
   
   for ( int arg = 0; arg < _arguments.size(); ++arg )
   {
      code += _arguments[arg];
      if ( arg < _arguments.size()-1 )
         code += ", ";
   }

   code += ")\n";
   code += getCode();
   code += "\nend";
   return code;
}

//////////////////////////////////////////////////////////////////////////
// - Storage interface
//////////////////////////////////////////////////////////////////////////

bool GuiEventHandlerDefinition::needsSave() const
{
   return !getCode().empty();
}

TiXmlElement* GuiEventHandlerDefinition::toXML() const
{
   TiXmlElement* pdefinition = new TiXmlElement("eventhandler");
   pdefinition->SetAttribute("name", getName());

   TiXmlElement* pcodeblock = new TiXmlElement("code");
   TiXmlText* pcode = new TiXmlText(getCode().c_str());
   pcodeblock->LinkEndChild(pcode);
   pdefinition->LinkEndChild(pcodeblock);  

   return pdefinition;
}

void GuiEventHandlerDefinition::fromXML(const TiXmlElement& element)
{
   setName(element.Attribute("name"));

   TiXmlElement* pcodeblock = (TiXmlElement*)element.FirstChild("code");
   if ( pcodeblock != NULL )
   {
      TiXmlText* pcode = (TiXmlText*)pcodeblock->FirstChild();
      setCode(pcode->Value());
   }
}

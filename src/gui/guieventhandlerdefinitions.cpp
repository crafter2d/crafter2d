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
#include <tinyxml.h>

#include "guieventhandlerdefinition.h"
#include "guiwnd.h"

#include "guieventhandlerdefinitions.h"
#ifndef JENGINE_INLINE
#  include "guieventhandlerdefinitions.inl"
#endif

GuiEventHandlerDefinitions::GuiEventHandlerDefinitions():
   _handlers()
{
}

//////////////////////////////////////////////////////////////////////////
// - Operations
//////////////////////////////////////////////////////////////////////////

void GuiEventHandlerDefinitions::add(GuiEventHandlerDefinition* pdefinition)
{
   getDefinitions().push_back(pdefinition);
}

GuiEventHandlerDefinition* GuiEventHandlerDefinitions::findByEventType(GuiEvent event)
{
   for ( int def = 0; def < getDefinitions().size(); ++def )
   {
      GuiEventHandlerDefinition* pdefinition = getDefinitions()[def];
      if ( pdefinition->getEvent() == event )
      {
         return pdefinition;
      }
   }

   return NULL;
}

GuiEventHandlerDefinition* GuiEventHandlerDefinitions::findByName(const std::string& name)
{
   for ( int def = 0; def < getDefinitions().size(); ++def )
   {
      GuiEventHandlerDefinition* pdefinition = getDefinitions()[def];
      if ( pdefinition->getName() == name )
      {
         return pdefinition;
      }
   }

   return NULL;
}

void GuiEventHandlerDefinitions::registerAll(GuiWnd& wnd)
{
   for ( int def = 0; def < getDefinitions().size(); ++def )
   {
      GuiEventHandlerDefinition* pdefinition = getDefinitions()[def];
      pdefinition->construct(wnd);
   }
}

//////////////////////////////////////////////////////////////////////////
// - Storage interface
//////////////////////////////////////////////////////////////////////////

TiXmlElement* GuiEventHandlerDefinitions::toXML() const
{
   TiXmlElement* pdefinitions = new TiXmlElement("eventhandlers");
   for ( int def = 0; def < getDefinitions().size(); ++def )
   {
      const GuiEventHandlerDefinition* pdefinition = getDefinitions()[def];
      if ( pdefinition->needsSave() )
      {
         TiXmlElement* phandler = pdefinition->toXML();
         pdefinitions->LinkEndChild(phandler);
      }
   }

   return pdefinitions;
}

void GuiEventHandlerDefinitions::fromXML(TiXmlElement* pelement)
{
   TiXmlElement* pdefinitions = (TiXmlElement*)pelement->FirstChild("eventhandlers");
   if ( pdefinitions != 0 )
   {
      GuiEventHandlerDefinition definition;
      TiXmlElement* pdefinition = (TiXmlElement*)pdefinitions->FirstChild("eventhandler");
      while ( pdefinition != NULL )
      {
         definition.fromXML(*pdefinition);

         GuiEventHandlerDefinition* poriginal = findByName(definition.getName());
         ASSERT_PTR(poriginal)

         poriginal->setCode(definition.getCode());

         pdefinition = (TiXmlElement*)pdefinitions->IterateChildren("eventhandler", pdefinition);
      }
   }
}

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
#ifndef GUIEVENTHANDLERDEFINITIONS_H_
#define GUIEVENTHANDLERDEFINITIONS_H_

#include <string>
#include <vector>
#include "guidefines.h"

class GuiEventHandlerDefinition;

/**
@author Jeroen Broekhuizen
*/
class GuiEventHandlerDefinitions
{
public:
   typedef std::vector<GuiEventHandlerDefinition*> Definitions;

   GuiEventHandlerDefinitions();

 // Get/set interface
   const Definitions&   getDefinitions() const;
         Definitions&   getDefinitions();

   GuiEventHandlerDefinition*  getDefinition(int index);

 // Operations
   void                        add(GuiEventHandlerDefinition* phandler);
   int                         size() const;

   GuiEventHandlerDefinition*  findByEventType(GuiEvent event);
   GuiEventHandlerDefinition*  findByName(const std::string& name);

   void                        registerAll(GuiWnd& wnd);

 // Storage interface
   TiXmlElement*  toXML() const;
   void           fromXML(TiXmlElement* pelement);

private:
   Definitions    _handlers;
};

#ifdef JENGINE_INLINE
#  include "guieventhandlerdefinitions.inl"
#endif

#endif

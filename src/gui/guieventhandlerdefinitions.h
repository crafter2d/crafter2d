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

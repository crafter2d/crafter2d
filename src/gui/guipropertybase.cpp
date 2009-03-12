/***************************************************************************
 *   Copyright (C) 2006 by Jeroen Broekhuizen                              *
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
#include <tinyxml.h>

#include "guiproperty.h"
#include "guiproperties.h"
#include "guipropertybase.h"
#ifndef JENGINE_INLINE
#  include "guipropertybase.inl"
#endif

GuiPropertyBase::GuiPropertyBase(std::string name, PropertyType type):
   MName(name),
   MType(type),
   MCallback(NULL),
   _pcontainer(NULL)
{
}

GuiPropertyBase::~GuiPropertyBase()
{
}

void GuiPropertyBase::registerCallback(CallbackFnc callback)
{
   MCallback = callback;
}

void GuiPropertyBase::callback()
{
   if ( MCallback != NULL )
      (*MCallback)(&container().getOwner());
}

GuiProperties& GuiPropertyBase::container()
{
   ASSERT_PTR(_pcontainer)
   return *_pcontainer;
}

void GuiPropertyBase::container(GuiProperties* pcontainer)
{
   _pcontainer = pcontainer;
}

//////////////////////////////////////////////////////////////////////////
// - Conversion
//////////////////////////////////////////////////////////////////////////

std::string GuiPropertyBase::toString() const
{
   switch ( Type() )
   {
   case EBool:
      {
         bool val = (dynamic_cast<const GuiProperty<bool>*>(this))->Value();
         return val ? "true" : "false";
      }
   case EString:
      {
         return (dynamic_cast<const GuiProperty<std::string>*>(this))->Value();
      }
   default: ASSERT(false)
   }

   return std::string();
}

void GuiPropertyBase::fromString(const char* pvalue)
{
   switch ( Type() )
   {
   case EBool:
      {
         bool value = false;
         if ( strncmp(pvalue, "true", 4) == 0 )
            value = true;
         (dynamic_cast<GuiProperty<bool>*>(this))->Value(value);
         break;
      }
   case EString:
      {
         (dynamic_cast<GuiProperty<std::string>*>(this))->Value(pvalue);
         break;
      }
   }
}

TiXmlElement* GuiPropertyBase::toXML() const
{
   TiXmlElement* pelement = new TiXmlElement("property");
   pelement->SetAttribute("name", Name().c_str());
   pelement->SetAttribute("value", toString().c_str());

   return pelement;
}

void GuiPropertyBase::fromXML(const TiXmlElement* pelement)
{
   const char *pname = pelement->Attribute("name");
   if ( strcmp(Name().c_str(), pname) == 0 )
   {
      fromString(pelement->Attribute("value"));
   }
}

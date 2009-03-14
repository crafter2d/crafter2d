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
#include "../defines.h"
#include <tinyxml.h>

#include "guipropertybase.h"
#include "guiproperties.h"

GuiProperties::GuiProperties():
   MProperties(),
   _powner(NULL)
{
}

GuiProperties::~GuiProperties()
{
}

void GuiProperties::Add(GuiPropertyBase* pproperty)
{
   MProperties.push_back(pproperty);
   pproperty->container(this);
}

void GuiProperties::clear()
{
   for ( int idx = 0; idx < MProperties.size(); ++idx )
   {
      delete MProperties[idx];
   }
   MProperties.clear();
}

int GuiProperties::Size() const
{
   return MProperties.size();
}

GuiPropertyBase* GuiProperties::find(const std::string& name)
{
   for ( int idx = 0; idx < MProperties.size(); ++idx )
   {
      GuiPropertyBase* pbase = MProperties[idx];
      if ( pbase->Name() == name )
      {
         return pbase;
      }
   }

   return NULL;
}

bool GuiProperties::hasOwner() const
{
   return _powner != NULL;
}

void GuiProperties::setOwner(GuiWnd* powner)
{
   _powner = powner;
}

GuiWnd& GuiProperties::getOwner()
{
   ASSERT(hasOwner())
   return *_powner;
}

const GuiPropertyBase& GuiProperties::operator[](int index) const
{
   ASSERT(index >= 0 && index < Size())
   return *MProperties[index];
}

GuiPropertyBase& GuiProperties::operator[](int index)
{
   return *MProperties[index];
}

//////////////////////////////////////////////////////////////////////////
// - Operations
//////////////////////////////////////////////////////////////////////////

TiXmlElement* GuiProperties::toXML() const
{
   TiXmlElement* pproperties = new TiXmlElement("properties");
   for ( int i = 0; i < MProperties.size(); ++i )
   {
      GuiPropertyBase* pbase = MProperties[i];
      pproperties->LinkEndChild(pbase->toXML());
   }

   return pproperties;
}

void GuiProperties::fromXML(const TiXmlElement* pelement)
{
   TiXmlElement* pproperties = (TiXmlElement*)pelement->FirstChild("properties");
   if ( pproperties != NULL )
   {
      TiXmlElement* pproperty   = (TiXmlElement*)pproperties->FirstChild("property");
      while ( pproperty )
      {
         std::string name = pproperty->Attribute("name");
         GuiPropertyBase* pbase = find(name);
         ASSERT_PTR(pbase);

         pbase->fromXML(pproperty);

         pproperty = (TiXmlElement*)pproperties->IterateChildren("property", pproperty);
      }
   }
}

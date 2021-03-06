/***************************************************************************
 *   Copyright (C) 2012 by Jeroen Broekhuizen                              *
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
#include "inifilesection.h"
#ifndef JENGINE_INLINE
#  include "inifilesection.inl"
#endif

#include "inifileexception.h"
#include "inifileproperty.h"

IniFileSection::IniFileSection(const String& name):
   mProperties(),
   mName(name)
{
}

IniFileSection::~IniFileSection()
{
   clear();
}

// - Query

const String& IniFileSection::get(const String& name)
{
   auto it = mProperties.find(name);
   if ( it == mProperties.end() )
   {
      throw new IniFileException(name + UTEXT(" is not a valid property."));
   }
   return it->second->getValue();
}

// - Operations

void IniFileSection::add(IniFileProperty* pproperty)
{
   mProperties.insert({ pproperty->getName(), pproperty });
}

void IniFileSection::clear()
{
   for ( auto& pair : mProperties )
   {
      delete pair.second;
   }
   mProperties.clear();
}

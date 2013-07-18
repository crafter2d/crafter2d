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
#ifndef INI_FILE_SECTION_H
#define INI_FILE_SECTION_H

#include "core/containers/hashmap.h"
#include "core/string/string.h"

class IniFileProperty;

class IniFileSection
{
public:
   explicit IniFileSection(const String& name);

 // get/set
   const String& getName() const;

 // query
   const String& get(const String& name);

 // operations
   void add(IniFileProperty* pproperty);

private:
 // types
   typedef HashMap<String, IniFileProperty*> Properties;

   Properties  mProperties;
   String      mName;
};

#ifdef JENGINE_INLINE
#  include "inifilesection.inl"
#endif

#endif // INI_FILE_SECTION_H
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
#ifndef INI_FILE_PROPERTY_H
#define INI_FILE_PROPERTY_H

#include "core/string/string.h"

class IniFileProperty
{
public:
   IniFileProperty(const String& name, const String& value);

 // get/set
   const String& getName() const;
   const String& getValue() const;

private:

   String mName;
   String mValue;
};

#ifdef JENGINE_INLINE
#  include "inifileproperty.inl"
#endif

#endif // INI_FILE_PROPERTY_H

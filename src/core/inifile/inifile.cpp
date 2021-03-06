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
#include "inifile.h"

#include "core/defines.h"
#include "core/string/char.h"
#include "core/string/string.h"
#include "core/vfs/filesystem.h"
#include "core/vfs/file.h"

#include "inifileexception.h"
#include "inifileparser.h"
#include "inifileproperty.h"
#include "inifilesection.h"

IniFile::IniFile(const String& filename):
   mSections()
{
   IniFileParser parser(filename, *this);
}

IniFile::~IniFile()
{
   clear();
}

// - Query

const String& IniFile::get(const String& name)
{
   return get(UTEXT("global"), name);
}

const String& IniFile::get(const String& section, const String& name)
{
   auto it = mSections.find(section);
   if ( it == mSections.end() )
   {
      throw new IniFileException(UTEXT("Section ") + name + UTEXT(" could not be found."));
   }

   IniFileSection* psection = it->second;
   ASSERT_PTR(psection);
   return psection->get(name);
}

/// - Operations

void IniFile::add(IniFileSection* psection)
{
   mSections.insert({ psection->getName(), psection });
}

void IniFile::clear()
{
   for ( auto& it : mSections )
   {
      delete it.second;
   }
   mSections.clear();
}

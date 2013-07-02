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
#include "inifileparser.h"

#include "core/string/char.h"
#include "core/string/string.h"
#include "core/vfs/filesystem.h"
#include "core/vfs/file.h"
#include "core/smartptr/autoptr.h"

#include "inifile.h"
#include "inifileproperty.h"
#include "inifilesection.h"

IniFileParser::IniFileParser(const  String& filename, IniFile& inifile)
{
   parse(filename, inifile);
}

void IniFileParser::parse(const String& filename, IniFile& inifile)
{
   IniFileSection* pcurrentsection = new IniFileSection(UTEXT("global"));
   inifile.mSections.insert(pcurrentsection->getName(), pcurrentsection);

   AutoPtr<File> file = FileSystem::getInstance().open(filename, File::ERead | File::EText);
   if ( file.hasPointer() )
   {
      skipWhiteSpace(*file);

      while ( !file->eof() )
      {
         char c = file->peekc();
         if ( c == '[' )
         {
            pcurrentsection = parseSection(*file);
            inifile.mSections.insert(pcurrentsection->getName(), pcurrentsection);
         }
         else if ( c == ';' )
         {
            parseComment(*file);
         }
         else
         {
            IniFileProperty* pproperty = parseProperty(*file);
            pcurrentsection->add(pproperty);
         }
      }
   }
}

IniFileSection* IniFileParser::parseSection(File& file)
{
   file.getc(); // skip [

   String name = parseWord(file, ']');

   file.getc(); // skip ]

   skipWhiteSpace(file);
   
   return new IniFileSection(name.trim());
}

IniFileProperty* IniFileParser::parseProperty(File& file)
{
   String name = parseWord(file, '=');

   char equals = file.getc();
   if ( equals != '=' )
   {
      return NULL;
   }

   skipWhiteSpace(file);
   String value = parseWord(file);

   return new IniFileProperty(name, value);
}

String IniFileParser::parseWord(File& file, char delim)
{
   String result;
   while ( !file.eof() )
   {
      char next = file.peekc();
      if ( next == EOF || next == delim || Char::isWhitespace(next) )
      {
         break;
      }

      result += file.getc();
   }

   skipWhiteSpace(file);

   return result;
}

void IniFileParser::parseComment(File& file)
{
   while ( !file.eof() && !Char::isEOL(file.getc()) );

   skipWhiteSpace(file);
}

void IniFileParser::skipWhiteSpace(File& file)
{
   for ( ; !file.eof() && Char::isWhitespace(file.peekc()); file.getc() );
}

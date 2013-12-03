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
#ifndef COMMAND_LINE_H
#define COMMAND_LINE_H

#include <vector>

#include "core/core_base.h"

#include "commandlineargument.h"

class CORE_API CommandLine
{
   typedef std::vector<CommandLineArgument> Arguments;

public:
   CommandLine(int argc, char *argv[]);
   ~CommandLine();

 // query
   int size() const;
   const CommandLineArgument& operator[](int index);

 // searching
   bool                       hasArgument(const String& name) const;
   const CommandLineArgument& getArgument(const String& name) const;
   const CommandLineArgument* findArgument(const String& name) const;

private:
 // parsing
   void parse(int argc, const char* const argv[]);

 // data
   Arguments   mArguments;
};

#endif // COMMAND_LINE_H

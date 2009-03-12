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
#ifndef SCRIPTFUNCTION_H
#define SCRIPTFUNCTION_H

#include <string>
#include <vector>

/**
@author Jeroen Broekhuizen
*/
class ScriptFunction
{
public:
   typedef std::vector<std::string> Arguments;

    ScriptFunction(std::string name);
    ScriptFunction(std::string name, std::string arg1);
    ScriptFunction(std::string name, std::string arg1, std::string arg2);
    ScriptFunction(std::string name, std::string arg1, std::string arg2, std::string arg3);
    ScriptFunction(std::string name, std::string arg1, std::string arg2, std::string arg3, std::string arg4);
    ~ScriptFunction();

private:
   std::string MName;
   Arguments   MArguments;
};

#endif

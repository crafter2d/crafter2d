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
#include "scriptfunction.h"

#include "../scriptmanager.h"

#include "scriptcollection.h"

ScriptFunction::ScriptFunction(std::string name):
   MName(name),
   MArguments()
{
   ScriptManager::getInstance().collections().back()->add(this);
}

ScriptFunction::ScriptFunction(std::string name, std::string arg1):
   MName(name),
   MArguments()
{
   ScriptManager::getInstance().collections().back()->add(this);
   MArguments.push_back(arg1);
}

ScriptFunction::ScriptFunction(std::string name, std::string arg1, std::string arg2):
   MName(name),
   MArguments()
{
   ScriptManager::getInstance().collections().back()->add(this);
   MArguments.push_back(arg1);
   MArguments.push_back(arg2);
}

ScriptFunction::ScriptFunction(std::string name, std::string arg1, std::string arg2, std::string arg3):
   MName(name),
   MArguments()
{
   ScriptManager::getInstance().collections().back()->add(this);
   MArguments.push_back(arg1);
   MArguments.push_back(arg2);
   MArguments.push_back(arg3);
}

ScriptFunction::ScriptFunction(std::string name, std::string arg1, std::string arg2,
                                                 std::string arg3, std::string arg4):
   MName(name),
   MArguments()
{
   ScriptManager::getInstance().collections().back()->add(this);
   MArguments.push_back(arg1);
   MArguments.push_back(arg2);
   MArguments.push_back(arg3);
   MArguments.push_back(arg4);
}

ScriptFunction::~ScriptFunction()
{
}

/***************************************************************************
 *   Copyright (C) 2009 by Jeroen Broekhuizen                              *
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
#include "scriptclass.h"

#include <tolua++.h>

#include "scriptlib.h"

ScriptClass::ScriptClass(ScriptLib& lib, const std::string& name, const std::string& base):
   mLib(lib)
{
   tolua_cclass(mLib.getLuaState(), name.c_str(), name.c_str(), base.c_str(), NULL);
   tolua_beginmodule(mLib.getLuaState(), name.c_str());
}

ScriptClass::~ScriptClass()
{
   tolua_endmodule(mLib.getLuaState());
}

void ScriptClass::addFunction(const std::string& name, pLuaFunction function)
{
   tolua_function(mLib.getLuaState(), name.c_str(), (lua_CFunction)function);
}

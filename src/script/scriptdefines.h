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
#ifndef SCRIPT_DEFINES_OF_JENGINE_H_
#define SCRIPT_DEFINES_OF_JENGINE_H_

#include <tolua++.h>
#include "scriptcollection.h"
#include "scriptfunction.h"

#define REGISTER_MODULE(class) \
   ScriptCollection* p##class_collection = ScriptManager::registerCollection(#class, tolua_reg_##class);

#define DEFINE_MODULE(class) \
   int tolua_reg_##class(lua_State* tolua_S);

#define REGISTER_CLASS(class,base)                                \
   static int tolua_collect_##class(lua_State* tolua_S)           \
   {                                                              \
      class* self = (class*)tolua_tousertype(tolua_S,1,0);        \
      delete self;                                                \
      return 0;                                                   \
   }                                                              \
   int tolua_reg_##class(lua_State* tolua_S)                      \
   {                                                              \
      tolua_open(tolua_S);                                        \
      tolua_usertype(tolua_S,#class);                             \
      tolua_module(tolua_S,NULL,0);                               \
      tolua_beginmodule(tolua_S,NULL);                            \
      tolua_usertype(tolua_S, #class);                            \
      tolua_cclass(tolua_S, #class, #class, #base, tolua_collect_##class); \
      tolua_beginmodule(tolua_S, #class);

#define REGISTER_END(class)                                       \
      tolua_endmodule(tolua_S);                                   \
      tolua_endmodule(tolua_S);                                   \
      return 1;                                                   \
   }

#define REGISTER_CONSTRUCTOR(class)                               \
      new ScriptFunction("new");                                  \
      tolua_function(tolua_S, "new", tolua_##class_new);

#define REGISTER_METHOD(class,fnc)                                \
      new ScriptFunction(#fnc);                                   \
      tolua_function(tolua_S, #fnc, tolua_##class_##fnc);

#define REGISTER_METHOD_1(class,fnc,arg1)                         \
      new ScriptFunction(#fnc, #arg1);                            \
      tolua_function(tolua_S, #fnc, tolua_##class_##fnc);

#define REGISTER_METHOD_2(class,fnc,arg1,arg2)                    \
      new ScriptFunction(#fnc, #arg1, #arg2);                     \
      tolua_function(tolua_S, #fnc, tolua_##class_##fnc);

#define REGISTER_METHOD_3(class,fnc,arg1,arg2,arg3)               \
      new ScriptFunction(#fnc, #arg1, #arg2, #arg3);              \
      tolua_function(tolua_S, #fnc, tolua_##class_##fnc);

#define REGISTER_METHOD_4(class,fnc,arg1,arg2,arg3,arg4)          \
      new ScriptFunction(#fnc, #arg1, #arg2, #arg3, #arg4);       \
      tolua_function(tolua_S, #fnc, tolua_##class_##fnc);

#define DEFINE_CONSTRUCTOR(class)                                 \
   int tolua_##class_new(lua_State* tolua_S)                      \
   {                                                              \
      tolua_pushusertype(tolua_S,(void*)new class(), #class);     \
      return 0;                                                   \
   }

#define DEFINE_METHOD(class,fnc)                                  \
   int tolua_##class_##fnc(lua_State* tolua_S)                    \
   {                                                              \
      class* pself = (class*)tolua_tousertype(tolua_S, 1, 0);     \
      pself->fnc(

#define DEFINE_END() ); return 0; }

#define STRING_ARG(nr)   (const char*)tolua_tostring(tolua_S,nr,0)
#define NUMBER_ARG(nr)   (int)tolua_tonumber(tolua_S, nr, 0)
#define BOOL_ARG(nr)     (bool)tolua_toboolean(tolua_S, nr, 0)

#define USER_ARG(type,nr)      (type*)tolua_tousertype(tolua_S, nr, 0)
#define USER_REF_ARG(type,nr)  *(type*)tolua_tousertype(tolua_S, nr, 0)

#endif //SCRIPT_DEFINES_OF_JENGINE_H_


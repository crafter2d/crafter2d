//
// C++ Interface: scriptcollection
//
// Description: 
//
//
// Author: Jeroen Broekhuizen <jeroen@jengine.homedns.org>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef SCRIPTCOLLECTION_H
#define SCRIPTCOLLECTION_H

#include <string>
#include <vector>

class ScriptFunction;
struct lua_State;

/**
@author Jeroen Broekhuizen
*/
class ScriptCollection
{
public:
   typedef int (*initializer)(lua_State* tolua_S);
   typedef std::vector<ScriptFunction*> Functions;

   ScriptCollection(std::string name, initializer function);
   ~ScriptCollection();

// Operations
   void initialize(lua_State* tolua_S);
   void destroy();
   
   void add(ScriptFunction* pfunction);

private:
   std::string MName;
   initializer MInitFunction;
   Functions   MFunctions;
};

#endif

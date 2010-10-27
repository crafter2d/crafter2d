/***************************************************************************
 *   Copyright (C) 2010 by Jeroen Broekhuizen                              *
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
#include "modifierai.h"

#include "core/script/script.h"
#include "core/script/scriptcontext.h"
#include "core/script/scriptmanager.h"

#include "object.h"
#include "process.h"

ModifierAI::ModifierAI(const std::string& function):
   Modifier(),
   mFunction(function)
{
}

ModifierAI::~ModifierAI()
{
}

void ModifierAI::update(Object& object, float delta)
{
   Process& process = object.getSceneGraph().getProcess();

   ScriptContext context;
   Script& script = process.getScriptManager().getTemporaryScript();
   script.setSelf(&object, "Object");
   script.prepareCall(mFunction.c_str());
   script.addParam(&object, "Object");
   script.addParam(delta);
   script.run(context, 2);
}

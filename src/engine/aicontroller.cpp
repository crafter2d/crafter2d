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
#include "aicontroller.h"

#include "core/defines.h"
#include "core/script/scriptobject.h"

#include "process.h"

namespace c2d
{

   // static
   const String AIController::smFunc("updateAI");

   AIController::AIController() :
      Controller(),
      mpScript(nullptr)
   {
   }

   AIController::~AIController()
   {
      delete mpScript;
      mpScript = nullptr;
   }

   // - Get/set

   void AIController::setThis(ScriptObject* pthis)
   {
      mpScript = pthis;
   }

   // - Operations

   void AIController::performAction(Entity& entity)
   {
      mpScript->prepareCall(1);
      mpScript->arg(0, &entity);
      mpScript->call(smFunc);
   }

} // namespace c2d

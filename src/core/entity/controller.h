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
#ifndef _CONTROLER_H_
#define _CONTROLER_H_

#include "core/core_base.h"

class ActionEvent;
class Entity;

namespace c2d
{
   /// @author Jeroen Broekhuizen
   ///
   /// The controler is the instance that has control over an entity. This
   /// is the base class for all controler types like user, ai, etc.
   class CORE_API Controller
   {
   public:
      Controller();
      virtual ~Controller();

      // operations
      virtual void requestAction(const ActionEvent& action);
      virtual void performAction(Entity& entity) = 0;
   };
}

#endif

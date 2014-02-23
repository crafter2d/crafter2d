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
#ifndef KEY_MAP_H_
#define KEY_MAP_H_

#include <map>

#include "core/defines.h"

namespace c2d
{
   class ActionMap;
   class Client;

   /// /brief Client-side mapping of the keys to actions
   ///
   /// The KeyMap is is used by the client to map key events to actions. These actions should be
   /// defines in the Lua code and also known on the server side.

   class KeyMap
   {
   public:
      KeyMap();

      // get/set
      void setClient(Client& client);

      // operations
      void bind(int key, int action);
      void update();

   private:
      struct KeyInfo
      {
         int action;
         bool state;
      };

      typedef std::map<int, KeyInfo> KeyInfos;

      Client*    mpClient;
      KeyInfos   mKeys;
   };
}

#endif // KEY_MAP_H_

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
#include "keymap.h"

#include "core/input/input.h"

#include "actionmap.h"
#include "client.h"

using namespace Input;

KeyMap::KeyMap():
   mpClient(NULL),
   mKeys()
{
}

// - Get/set

void KeyMap::setClient(Client& client)
{
   mpClient = &client;
}

// - Operations

void KeyMap::bind(int key, int action)
{
   KeyInfo info;
   info.action = action;
   info.state  = false;   

   mKeys[key] = info;
}

void KeyMap::update()
{
   ASSERT_PTR(mpClient)

   ActionMap* pactionmap = mpClient->getActionMap();
   if ( pactionmap != NULL )
   {
      InputDevice& input = mpClient->getInput();

      KeyInfos::iterator it = mKeys.begin();
      for ( ; it != mKeys.end(); ++it )
      {
         int key = it->first;
         KeyInfo& info = it->second;

         if ( input.isKeyDown(key) )
         {
            if ( !info.state )
            {
               info.state = true;
               pactionmap->process(info.action, true);
            }
         }
         else if ( info.state )
         {
            info.state = false;
            pactionmap->process(info.action, false);
         }
      }
   }
}

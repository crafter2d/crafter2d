/***************************************************************************
 *   Copyright (C) 2011 by Jeroen Broekhuizen                              *
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
#ifndef CLIENT_MOUSE_EVENT_DISPATCHER_H_
#define CLIENT_MOUSE_EVENT_DISPATCHER_H_

#include "core/input/mouseeventdispatcher.h"

namespace c2d
{
   class Client;

   class ClientMouseEventDispatcher : public Input::MouseEventDispatcher
   {
   public:
      explicit ClientMouseEventDispatcher(Client& client);
      virtual ~ClientMouseEventDispatcher();

      // overloads
      virtual void dispatch(const Input::MouseEvent& event);

   private:

      // members
      Client& mClient;
   };
}

#endif // CLIENT_MOUSE_EVENT_DISPATCHER_H_

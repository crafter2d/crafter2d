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
#include "netclients.h"

#include "netaddress.h"

NetClients::NetClients():
   mClients(),
   mNextId(0)
{
}

NetClients::~NetClients()
{
   clear();
}

NetAddress& NetClients::operator[](int index)
{
   return *mClients[index];
}

// - Query

void NetClients::rewind()
{
   mIterator = mClients.begin();
}

bool NetClients::hasNext()
{
   return mIterator != mClients.end();
}

NetAddress& NetClients::getNext()
{
   NetAddress* paddress = mIterator->second;
   ++mIterator;
   return *paddress;
}

// - Maintenance

NetAddress& NetClients::add(NetAddress* paddress)
{
   paddress->index = mNextId++;
   mClients[paddress->index] = paddress;
   return *paddress;
}

void NetClients::remove(int clientid)
{
   ClientMap::iterator it = mClients.find(clientid);
   if ( it != mClients.end() )
   {
      delete mClients[clientid];
      mClients.erase(it);
   }
}

void NetClients::clear()
{
   for ( std::size_t index = 0; index < mClients.size(); index++ )
   {
      NetAddress* paddress = mClients[index];
      delete paddress;
   }
   mClients.clear();
}

// - Search

NetAddress* NetClients::find(const NetAddress& address)
{
   for ( ClientMap::iterator it = mClients.begin(); it != mClients.end(); ++it )
   {
      NetAddress* pclient = it->second;
#ifdef WIN32
      if ( pclient->addr.sin_addr.S_un.S_addr == address.addr.sin_addr.S_un.S_addr )
#else
      if ( pclient->addr.sin_addr.s_addr == address.addr.sin_addr.s_addr )
#endif
      {
         return pclient;
      }
   }

   return NULL;
}


#ifndef NET_CLIENTS_H
#define NET_CLIENTS_H

#include <vector>
#include <map>
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
class NetAddress;

class NetClients
{
   typedef std::map<int, NetAddress*>  ClientMap;
   typedef ClientMap::iterator         Iterator;

public:
   NetClients();
   ~NetClients();
   NetAddress& operator[](int index);

 // iteration
   void        rewind();
   bool        hasNext();
   NetAddress& getNext();

 // maintenance
   NetAddress& add(NetAddress* paddress);
   void        remove(int clientid);
   void        clear();

 // searching
   NetAddress* find(const NetAddress& address);

private:
   ClientMap mClients;
   Iterator  mIterator;
   int       mNextId;
};

#endif // NET_CLIENTS_H

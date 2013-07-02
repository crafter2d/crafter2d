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
#include "netsocket.h"

#ifdef WIN32
#include <ws2tcpip.h>
#else
#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#define SOCKET_ERROR -1
#endif

#include "core/string/string.h"

#include "netaddress.h"
#include "netpackage.h"
#include "netsocketexception.h"

NetSocket::NetSocket():
   mSocket(0)
{
}

// - Query
   
bool NetSocket::resolve(NetAddress& address, const String& name)
{
   const std::string addr = name.toUtf8();

   address.addr.sin_family = AF_INET;
	address.addr.sin_port = htons ((u_short)0);
   address.addr.sin_addr.s_addr = inet_addr(addr.c_str());

	if (address.addr.sin_addr.s_addr == INADDR_NONE)
   {
      hostent *host_info = gethostbyname(addr.c_str());
		if (host_info == NULL)
      {
			return false;
      }

		memcpy(&address.addr.sin_addr, host_info->h_addr, host_info->h_length);
	}

   return true;
}

// operations

bool NetSocket::create()
{
   mSocket = (int)socket(AF_INET, SOCK_DGRAM, 0);
   if ( mSocket == -1 )
   {
      return false;
   }

   // set the socket to non blocking
#ifdef WIN32
   unsigned long non_block = 1;
   ioctlsocket (mSocket, FIONBIO, &non_block);
#else
   fcntl (mSock, F_SETFL, O_NONBLOCK );
#endif

   return true;
}

bool NetSocket::bind(int port)
{
   if ( !create() )
   {
      return false;
   }

   sockaddr_in sa;
   sa.sin_family = AF_INET;
   sa.sin_port = htons(port);
   sa.sin_addr.s_addr = htonl(INADDR_ANY);

   if ( ::bind(mSocket, (sockaddr*)&sa, sizeof(sa)) < 0 )
   {
      return false;
   }

   return true;
}

bool NetSocket::connect(NetAddress& to, int port)
{
   if ( !bind(0) )
   {
      return false;
   }

   to.addr.sin_port = htons ((u_short)port);

   return true;
}

void NetSocket::close()
{
   // close the socket connection
#ifdef WIN32
      shutdown(mSocket, SD_BOTH);
      closesocket(mSocket);
#else
      shutdown(mSocket, SHUT_RDWR);
      close(mSocket);
#endif
}

bool NetSocket::select(bool read, bool write)
{
   fd_set readSet;
   struct timeval timeVal;

   FD_ZERO(&readSet);
   FD_SET(mSocket, &readSet);
   timeVal.tv_sec = 0;
   timeVal.tv_usec = 0;

   // poll (non-blocking) if there is data waiting
   return (::select(mSocket+1, &readSet, NULL, NULL, &timeVal) > 0);
}

int NetSocket::receive(NetAddress& from, NetPackage& package)
{
   int addrlen = NetAddress::SOCKADDR_SIZE;
   int bytes = recvfrom(mSocket, (char*)&package, NetPackage::MaxPackageSize, 0, (struct sockaddr*)&(from.addr), (socklen_t*)&addrlen);
   if ( bytes == SOCKET_ERROR )
   {
      handleError();  
   }
   return bytes;
}
   
int NetSocket::send(NetAddress& to, const NetPackage& package)
{
   int size = package.getSize();
   int bytes = sendto(mSocket, (const char*)&package, size, 0, (const sockaddr*) &to.addr, NetAddress::SOCKADDR_SIZE);
   if ( bytes == SOCKET_ERROR )
   {
      handleError();
   }
   return bytes;
}

// - Error handling

/// \fn NetSocket::getErrorNumber()
/// \brief Returns the error number when a socket function failed. Currently, this function
/// is only supported on the Windows platform.
void NetSocket::handleError() const
{
#ifdef WIN32
   int platformerror = WSAGetLastError();
   NetSocketException::Error error = NetSocketException::eUnsupportedError;
   switch ( WSAGetLastError() )
   {
   case WSAECONNRESET:
      error = NetSocketException::eConnReset;
      break;
   default:
      break;
   }
#else
   int platformerror = -1;
#endif

   throw new NetSocketException(error, platformerror);
}

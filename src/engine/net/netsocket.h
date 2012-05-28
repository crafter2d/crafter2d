
#ifndef NET_SOCKET_H
#define NET_SOCKET_H

#include <string>

class NetAddress;
class NetPackage;

class NetSocket
{
public:
   NetSocket();

 // query
   bool resolve(NetAddress& address, const std::string& name);

 // operations
   bool bind(int port);
   bool connect(NetAddress& to, int port);
   void close();

   bool select(bool read, bool write);

   int receive(NetAddress& from, NetPackage& package);
   int send(NetAddress& to, const NetPackage& package);

private:
 // operations
   bool create();

 // error handling
   void handleError() const;

   int mSocket;
};

#endif // UDP_SOCKET_H

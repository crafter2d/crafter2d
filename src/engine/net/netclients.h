
#ifndef NET_CLIENTS_H
#define NET_CLIENTS_H

#include <vector>

class NetAddress;

class NetClients
{
   typedef std::vector<NetAddress*> Clients;

public:
   NetClients();
   ~NetClients();

 // query
   int size() const;
   const NetAddress& operator[](int index) const;
         NetAddress& operator[](int index);

   int indexOf(const NetAddress& address) const;

   bool contains(const NetAddress& address) const;

 // maintenance
   int add(NetAddress* paddress);
   void remove(int clientid);
   void clear();

 // search
   NetAddress* find(const NetAddress& address);

private:
   Clients mClients;
};

#endif // NET_CLIENTS_H


#include "netclients.h"

#include "netaddress.h"

NetClients::NetClients():
   mClients()
{
}

NetClients::~NetClients()
{
   clear();
}

// - Query
int NetClients::size() const
{
   return mClients.size();
}

const NetAddress& NetClients::operator[](int index) const
{
   return *mClients[index];
}

NetAddress& NetClients::operator[](int index)
{
   return *mClients[index];
}

int NetClients::indexOf(const NetAddress& address) const
{
   for ( int i = 0 ; i < mClients.size(); ++i )
   {
      NetAddress* pclient = mClients[i];
#ifdef WIN32
      if ( pclient->addr.sin_addr.S_un.S_addr == address.addr.sin_addr.S_un.S_addr )
#else
      if ( pclient->addr.sin_addr.s_addr == address.addr.sin_addr.s_addr )
#endif
      {
         return i;
      }
   }
   return -1;
}

bool NetClients::contains(const NetAddress& address) const
{
   return indexOf(address) != -1;
}

// - Maintenance

int NetClients::add(NetAddress* paddress)
{
   for ( std::size_t index = 0; index < mClients.size(); index++ )
   {
      if ( mClients[index] != NULL )
      {
         mClients[index] = paddress;
         return index;
      }
   }

   mClients.push_back(paddress);
   return mClients.size() - 1;
}

void NetClients::remove(int clientid)
{
   ASSERT_PTR(mClients[clientid]);
   delete mClients[clientid];
   mClients[clientid] = NULL;
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
   int index = indexOf(address);
   return index != -1 ? mClients[index] : NULL;
}

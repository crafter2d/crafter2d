
#include "netaddress.h"

INLINE
NetAddress::NetAddress():
   packageNumber(1),
   nextPackageNumber(1),
   lastTimeRecv(0),
   lastTimeSend(0),
   waitAttempt(0),
   waitTimer(0),
   orderQueue(),
   resendQueue(),
   pstatistics(0)
{
   memset(&addr, 0, SOCKADDR_SIZE);
   addr.sin_family = AF_INET;
}

INLINE
NetAddress::NetAddress(const sockaddr_in& address):
   packageNumber(1),
   nextPackageNumber(1),
   lastTimeRecv(0),
   lastTimeSend(0),
   waitAttempt(0),
   waitTimer(0),
   orderQueue(),
   resendQueue(),
   pstatistics(0)
{
   memcpy(&addr, &address, SOCKADDR_SIZE);
}

// operations

void NetAddress::removeAcknowledged(uint number)
{
   std::size_t index = 0;
   for ( ; index < resendQueue.size(); ++index )
   {
      NetPackage* ppackage = resendQueue[index];
      if ( ppackage->getNumber() >= number )
      {
         break;
      }
   }

   if ( index < resendQueue.size() )
   {
      resendQueue.erase(resendQueue.begin(), resendQueue.begin() + index);
   }
}
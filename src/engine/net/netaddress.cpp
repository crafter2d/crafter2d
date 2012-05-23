
#include "netaddress.h"

INLINE
NetAddress::NetAddress():
   packageNumber(1),
   lastPackageNumber(0),
   lastTimeRecv(0),
   lastTimeSend(0),
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
   lastPackageNumber(0),
   lastTimeRecv(0),
   lastTimeSend(0),
   orderQueue(),
   resendQueue(),
   pstatistics(0)
{
   memcpy(&addr, &address, SOCKADDR_SIZE);
}

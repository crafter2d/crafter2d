
#ifndef NET_ADDRESS_H
#define NET_ADDRESS_H

#ifdef WIN32
#include <winsock2.h>
#else
#include <netinet/in.h>
#endif
#include <vector>

#include "netpackage.h"
#include "sortedpackagelist.h"

class NetPackage;
class NetStatistics;

class NetAddress
{
public:
   static const int SOCKADDR_SIZE = sizeof(sockaddr_in);

   typedef std::vector<NetPackage*> PackageQueue;

   NetAddress();
   NetAddress(const sockaddr_in& address);

 // operations
   void removeAcknowledged(uint number);

 // members
   sockaddr_in addr;

   uint  packageNumber;
   uint  nextPackageNumber;
   float lastTimeRecv;
   float lastTimeSend;

   int   waitAttempt;
   float waitTimer;

   SortedPackageList orderQueue;
   PackageQueue resendQueue;

   NetStatistics* pstatistics;
};

#endif // NET_ADDRESS_H

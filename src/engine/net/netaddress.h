
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

const int   SOCKADDR_SIZE                 = sizeof(sockaddr_in);

class NetAddress
{
public:
   typedef std::vector<NetPackage*> PackageQueue;

   NetAddress();
   NetAddress(const sockaddr_in& address);

   sockaddr_in addr;

   uint  packageNumber;
   uint  lastPackageNumber;
   float lastTimeRecv;
   float lastTimeSend;

   SortedPackageList orderQueue;
   PackageQueue resendQueue;

   NetStatistics* pstatistics;
};

#endif // NET_ADDRESS_H

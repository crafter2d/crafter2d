
#include "netsocketexception.h"

NetSocketException::NetSocketException(Error error, int platformerror):
   mError(error),
   mPlatformError(platformerror)
{
}

// - Get/set

NetSocketException::Error NetSocketException::getError() const
{
   return mError;
}

int NetSocketException::getPlatformError() const
{
   return mPlatformError;
}

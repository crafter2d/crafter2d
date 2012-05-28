
#ifndef NET_SOCKET_EXCEPTION_H
#define NET_SOCKET_EXCEPTION_H

class NetSocketException
{
public:
   enum Error
   {
      eConnReset,
      eUnsupportedError
   };

   NetSocketException(Error error, int platformerror);

 // get/set
   Error getError() const;
   int   getPlatformError() const;

private:
   Error mError;
   int   mPlatformError;
};

#endif // NET_SOCKET_EXCEPTION_H


#include "exception.h"

namespace c2d
{
   Exception::Exception():
      mReason()
   {
   }
   
   Exception::Exception(const String& reason):
      mReason(reason)
   {
   }
   
   Exception::Exception(const Exception& that):
      mReason(that.mReason)
   {
   }
   
   // - Get/set
   
   const String& Exception::getReason() const
   {
      return mReason;
   }
}

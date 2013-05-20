
#include "exceptionguards.h"

namespace ByteCode
{
   ExceptionGuards::ExceptionGuards():
      mGuards()
   {
   }

   const ExceptionGuard& ExceptionGuards::operator[](int index) const
   {
      return *mGuards[index];
   }

   ExceptionGuard& ExceptionGuards::operator[](int index)
   {
      return *mGuards[index];
   }

   // - Maintenance
      
   void ExceptionGuards::add(ExceptionGuard* pguard)
   {
      mGuards.push_back(pguard);
   }
}
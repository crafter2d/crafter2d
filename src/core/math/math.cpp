
#include "math.h"

int Math::nextPowerOfTwo(int value)
{
   int newsize = 1;
   while ( newsize < value )
   {
      newsize <<= 1;
   }

   return newsize;
}

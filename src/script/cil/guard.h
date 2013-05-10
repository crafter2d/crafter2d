
#ifndef CIL_GUARD_H
#define CIL_GUARD_H

namespace CIL
{
   class Guard
   {
   public:
      bool finalize;
      int  size;
      int  labels[3];
   };
}

#endif // CIL_GUARD_H

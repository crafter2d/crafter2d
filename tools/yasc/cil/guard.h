
#ifndef CIL_GUARD_H
#define CIL_GUARD_H

namespace CIL
{
   class Guard
   {
   public:
      static const int sStart = 0;
      static const int sCatch = 1;
      static const int sFinal = 2;
      static const int sEnd   = 3;

      Guard* clone() const;

      bool finalize;
      int  labels[4];
   };
}

#endif // CIL_GUARD_H

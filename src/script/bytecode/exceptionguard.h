
#ifndef BYTECODE_EXCEPTIONGUARD_H
#define BYTECODE_EXCEPTIONGUARD_H

namespace ByteCode
{
   class ExceptionGuard
   {
   public:
      static const int sStart = 0;
      static const int sCatch = 1;
      static const int sFinal = 2;
      static const int sEnd   = 3;

      bool finalize;
      int  locations[4];
   };
}

#endif // BYTECODE_EXCEPTIONGUARD_H


#ifndef BYTECODE_EXCEPTIONGUARDS_H
#define BYTECODE_EXCEPTIONGUARDS_H

#include <vector>

namespace ByteCode
{
   class ExceptionGuard;

   class ExceptionGuards
   {
      typedef std::vector<ExceptionGuard*> GuardVector;

   public:
      ExceptionGuards();

      const ExceptionGuard& operator[](int index) const;
            ExceptionGuard& operator[](int index);

    // maintenance
      void add(ExceptionGuard* pguard);

   private:
      GuardVector mGuards;
   };
}

#endif // BYTECODE_EXCEPTIONGUARDS_H

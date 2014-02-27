
#ifndef CIL_INSTRUCTIONS_H
#define CIL_INSTRUCTIONS_H

#include <vector>

#include "cil.h"

namespace CIL
{
   class Instructions
   {
      typedef std::vector<Instruction> Insts;

   public:

      const Instructions& operator=(const Instructions& that);

      const Instruction& operator[](int index) const;
            Instruction& operator[](int index);
    // query
      bool isEmpty() const;
      int size() const;

    // operations
      void add(const Instruction& inst);

   private:

    // data
      Insts mInsts;
   };
};

#endif // CIL_INSTRUCTIONS_H

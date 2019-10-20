
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

      const Instruction& operator[](int index) const
      {
         return mInsts[index];
      }
      
      Instruction& operator[](int index)
      {
         return mInsts[index];
      }
      
    // query
      bool isEmpty() const
      {
         return size() == 0;
      }

      std::size_t size() const
      {
         return mInsts.size();
      }

    // operations
      void add(const Instruction& inst);
      void clear();

   private:

    // data
      Insts mInsts;
   };
};

#endif // CIL_INSTRUCTIONS_H

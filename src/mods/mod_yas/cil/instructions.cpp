
#include "instructions.h"

namespace CIL
{

   const Instructions& Instructions::operator=(const Instructions& that)
   {
      mInsts.resize(that.size());
      for ( int index = 0; index < that.size(); ++index )
      {
         mInsts[index] = that[index];
      }
      return *this;
   }

   const Instruction& Instructions::operator[](int index) const
   {
      return mInsts[index];
   }

   Instruction& Instructions::operator[](int index)
   {
      return mInsts[index];
   }

   // - Query

   bool Instructions::isEmpty() const
   {
      return mInsts.empty();
   }

   size_t Instructions::size() const
   {
      return mInsts.size();
   }

   // - Operations

   void Instructions::add(const Instruction& inst)
   {
      mInsts.push_back(inst);
   }
}

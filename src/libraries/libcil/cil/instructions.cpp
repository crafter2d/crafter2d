
#include "instructions.h"

namespace CIL
{

   const Instructions& Instructions::operator=(const Instructions& that)
   {
      mInsts = that.mInsts;
      /*
      mInsts.resize(that.size());
      for ( int index = 0; index < that.size(); ++index )
      {
         mInsts[index] = that[index];
      }
      */
      return *this;
   }

   // - Operations

   void Instructions::add(const Instruction& inst)
   {
      mInsts.push_back(inst);
   }

   void Instructions::clear()
   {
      mInsts.clear();
   }
}

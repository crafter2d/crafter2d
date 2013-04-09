
#ifndef CIL_SWITCH_TABLE_H
#define CIL_SWITCH_TABLE_H

#include <vector>

#include "script/common/variant.h"

namespace CIL
{
   class SwitchTable
   {
   public:
      SwitchTable();

    // operations
      void add(int label, const Variant& value);
      void addDefault(int label);
      void addEnd(int label);

   private:
      struct Case
      {
         int label;
         Variant value;
      };
      typedef std::vector<Case> Cases;

      Cases mCases;
      int   mDefault;
      int   mEnd;
   };
}

#endif // CIL_SWITCH_TABLE_H

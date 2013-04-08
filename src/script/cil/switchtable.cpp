
#include "switchtabel.h" 

#include "script/common/variant.h"

namespace CIL
{

   SwitchTable::SwitchTable():
      mCases(),
      mDefault(-1),
      mEnd(-1)
   {
   }

   void SwitchTable::add(int label, const Variant& value)
   {
      Case c;
      c.label = label;
      c.value = value;
      mCases.push_back(c);
   }
      
   void SwitchTable::addDefault(int label)
   {
      mDefault = label;
   }

   void SwitchTable::addEnd(int label)
   {
      mEnd = label;
   }
}

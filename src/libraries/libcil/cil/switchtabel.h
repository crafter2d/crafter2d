
#ifndef CIL_SWITCH_TABLE_H
#define CIL_SWITCH_TABLE_H

#include <vector>

#include "core/system/variant.h"

namespace CIL
{
   class SwitchTableEntry;

   class SwitchTable
   {
   public:
      SwitchTable();

    // operators
      const SwitchTableEntry& operator[](int index) const;
            SwitchTableEntry& operator[](int index);

    // query
      int size() const;

      bool hasDefault() const;
      int  getDefault() const;

      int getEnd() const;

    // operations
      void add(const SwitchTableEntry& entry);
      void addDefault(int label);
      void addEnd(int label);

   private:
      typedef std::vector<SwitchTableEntry> Cases;

      Cases mCases;
      int   mDefault;
      int   mEnd;
   };
}

#endif // CIL_SWITCH_TABLE_H


#ifndef VIRTUAL_LOOKUP_TABLE_H_
#define VIRTUAL_LOOKUP_TABLE_H_

#include <map>

#include "script/common/variant.h"

class VirtualLookupTable
{
   class Entry
   {
   public:
      Entry(int index): mActual(index) {}

      int mActual;
   };

   typedef std::map<Variant, Entry*> EntryMap;

public:
   VirtualLookupTable();

 // get/set
   void setDefault(int codeindex);
   void setEnd(int codeindex);

 // query
   int lookup(const Variant& value) const;
   
 // operations
   void add(const Variant& variant, int codeindex);
   void offsetCode(int offset);

private:
   EntryMap mEntries;
   int      mDefault;
   int      mEnd;
};

#endif // VIRTUAL_LOOKUP_TABLE_H_

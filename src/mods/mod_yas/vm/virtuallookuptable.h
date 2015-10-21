
#ifndef VIRTUAL_LOOKUP_TABLE_H_
#define VIRTUAL_LOOKUP_TABLE_H_

#include <vector>
#include <map>

#include "virtualvalue.h"

class VirtualLookupTable
{
   typedef std::map<VirtualValue, int> EntryMap;

public:
   VirtualLookupTable();

 // get/set
   bool hasDefault() const;
   int  getDefault() const;
   void setDefault(int codeindex);

   int  getEnd() const;
   void setEnd(int codeindex);

 // query
   int lookup(const VirtualValue& value) const;
   const VirtualValue* lookup(int pos) const;

   std::vector<int> getPositions() const;
      
 // operations
   void add(const VirtualValue& variant, int codeindex);
   void updatePosition(int offset);

private:

 // data
   EntryMap mEntries;
   int      mDefault;
   int      mEnd;
};

#endif // VIRTUAL_LOOKUP_TABLE_H_

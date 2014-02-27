
#ifndef VIRTUAL_LOOKUPTABLES_H
#define VIRTUAL_LOOKUPTABLES_H

#include <vector>

class VirtualLookupTable;

class VirtualLookupTables
{
   typedef std::vector<VirtualLookupTable*> Tables;

public:

   ~VirtualLookupTables();

   const VirtualLookupTable& operator[](int index) const;
         VirtualLookupTable& operator[](int index);

 // query
   int size() const;

 // operations
   void add(VirtualLookupTable* ptable);
   void clear();

private:

 // data
   Tables mTables;
};

#endif // VIRTUAL_LOOKUPTABLES_H


#ifndef VIRTUAL_FUNCTION_TABLE_H_
#define VIRTUAL_FUNCTION_TABLE_H_

#include <vector>

class String;
class VirtualFunction;
class VirtualFunctionTableEntry;

class VirtualFunctionTable
{
   typedef std::vector<VirtualFunctionTableEntry*> Entries;

public:
   VirtualFunctionTable();
   ~VirtualFunctionTable();

   const VirtualFunctionTable& operator=(const VirtualFunctionTable& that);

 // query
   int size() const;

   const VirtualFunctionTableEntry& operator[](int index) const;
         VirtualFunctionTableEntry& operator[](int index);

 // operations
   void setInits(VirtualFunction& static_init, VirtualFunction& var_init);
   int  insert(VirtualFunction& function);
   void clear();
   void update();

 // search
   const VirtualFunctionTableEntry* findByName(const String& name) const;

private:

 // data
   Entries mEntries;
};

#endif // VIRTUAL_FUNCTION_TABLE_H_

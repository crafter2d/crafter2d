
#ifndef VIRTUAL_FUNCTION_TABLE_H_
#define VIRTUAL_FUNCTION_TABLE_H_

#include <string>
#include <vector>

class VirtualFunctionTableEntry;

class VirtualFunctionTable
{
   typedef std::vector<VirtualFunctionTableEntry*> Entries;

public:
   VirtualFunctionTable();
   ~VirtualFunctionTable();

 // query
   int size() const;

   const VirtualFunctionTableEntry& operator[](int index) const;
         VirtualFunctionTableEntry& operator[](int index);

 // operations
   void append(VirtualFunctionTableEntry* pentry);
   void append(const VirtualFunctionTable& table);
   void clear();

   void merge(const VirtualFunctionTable& that);
   void offset(int offset);

 // search
   const VirtualFunctionTableEntry* findByName(const std::string& name) const;

   const VirtualFunctionTableEntry& resolveInterface(int index) const;

private:
   Entries mEntries;
};

#endif // VIRTUAL_FUNCTION_TABLE_H_


#ifndef VIRTUAL_CLASS_TABLE_H_
#define VIRTUAL_CLASS_TABLE_H_

#include <unordered_map>
#include <vector>

#include "core/string/string.h"

class LiteralTable;
class VirtualClass;

class VirtualClassTable
{
   typedef std::unordered_map<String, VirtualClass*> Classes;

public:
   VirtualClassTable();
   ~VirtualClassTable();

 // query
   bool contains(const String& name) const;
   
 // operations
   void clear();
   void insert(VirtualClass* ptype);
   void print(const LiteralTable& literals);
   void mark();

 // search
   VirtualClass* find(const String& name) const;

   const VirtualClass& resolve(const String& name) const;
         VirtualClass& resolve(const String& name);

 // conversion
   std::vector<VirtualClass*> asArray();
   
private:

   Classes mClasses;
};

#endif // VIRTUAL_CLASS_TABLE_H_


#ifndef VIRTUAL_CLASS_TABLE_H_
#define VIRTUAL_CLASS_TABLE_H_

#include <map>
#include <string>
#include <vector>

class LiteralTable;
class VirtualClass;

class VirtualClassTable
{
   typedef std::map<std::string, VirtualClass*> Classes;

public:
   VirtualClassTable();

 // query
   bool contains(const std::string& name) const;
   
 // operations
   void insert(VirtualClass* ptype);
   void print(const LiteralTable& literals);

 // search
   VirtualClass* find(const std::string& name) const;

   const VirtualClass& resolve(const std::string& name) const;
         VirtualClass& resolve(const std::string& name);

 // conversion
   std::vector<VirtualClass*> asArray();
   
private:
   void resolveTypes();

   Classes mClasses;
};

#endif // VIRTUAL_CLASS_TABLE_H_


#include "virtualclasstable.h"

#include "core/defines.h"

#include "virtualclass.h"

#include <iostream>
#include <algorithm>


VirtualClassTable::VirtualClassTable():
   mClasses()
{
}

// - Query
   
bool VirtualClassTable::contains(const std::string& name) const
{
   return find(name) != NULL;
}

// - Operations

void VirtualClassTable::insert(VirtualClass* ptype)
{
   mClasses[ptype->getName()] = ptype;
}

void VirtualClassTable::print(const LiteralTable& literals)
{
   using std::cout;

   Classes::iterator it = mClasses.begin();
   for ( ; it != mClasses.end(); it++ )
   {
      VirtualClass* pclass = it->second;
      std::string code = pclass->getInstructions().toString(literals);

      cout << "=====================================" << std::endl;
      cout << "Code of " << pclass->getName() << std::endl << std::endl;
      cout << code << std::endl;
   }
}

// - Search

VirtualClass* VirtualClassTable::find(const std::string& name) const
{
   Classes::const_iterator it = mClasses.find(name);
   return it != mClasses.end() ? it->second : NULL;
}

const VirtualClass& VirtualClassTable::resolve(const std::string& name) const
{
   return const_cast<VirtualClassTable&>(*this).resolve(name);
}

VirtualClass& VirtualClassTable::resolve(const std::string& name)
{
   VirtualClass* pclass = find(name);
   ASSERT_PTR(pclass);
   return *pclass;
}


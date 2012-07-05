
#include "virtualclasstable.h"

#include "core/defines.h"

#include "script/common/variant.h"

#include "virtualclass.h"
#include "virtualobject.h"

#include <iostream>
#include <algorithm>


VirtualClassTable::VirtualClassTable():
   mClasses()
{
}

VirtualClassTable::~VirtualClassTable()
{
   clear();
}

// - Query

bool VirtualClassTable::contains(const std::string& name) const
{
   return find(name) != NULL;
}

// - Operations

void VirtualClassTable::clear()
{
   Classes::iterator it = mClasses.begin();
   for ( ; it != mClasses.end(); it++ )
   {
      delete it->second;
   }

   mClasses.clear();
}

void VirtualClassTable::insert(VirtualClass* ptype)
{
   mClasses[ptype->getName()] = ptype;
}

void VirtualClassTable::mark()
{
   Classes::iterator it = mClasses.begin();
   for ( ; it != mClasses.end(); it++ )
   {
      VirtualClass* pclass = it->second;
      int statics = pclass->getStaticCount();
      for ( int index = 0; index < statics; index++ )
      {
         Variant& var = pclass->getStatic(index);
         if ( var.isObject() )
         {
            var.asObject().mark();
         }
      }
   }
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

// - Conversion

std::vector<VirtualClass*> VirtualClassTable::asArray()
{
   std::vector<VirtualClass*> array;
   Classes::iterator it = mClasses.begin();
   for ( ; it != mClasses.end(); it++ )
   {
      VirtualClass* pclass = it->second;
      array.push_back(pclass);
   }
   return array;
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


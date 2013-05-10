
#include "virtualclass.h"

#include "core/defines.h"

#include "script/ast/astclass.h"
#include "script/common/variant.h"

#include "virtualobject.h"
#include "virtualfunctiontable.h"
#include "virtuallookuptable.h"

VirtualClass::VirtualClass():
   mName(),
   mBaseName(),
   mpBaseClass(NULL),
   mpDefinition(NULL),
   mVTable(),
   mpByteCode(NULL),
   mpClassObject(NULL),
   mpStatics(NULL),
   mStaticCount(0),
   mVariableCount(0),
   mFlags(eNone)
{
}

VirtualClass::~VirtualClass()
{
   setDefinition(NULL);

   delete[] mpStatics;
   mpStatics = NULL;
}

// - Get/set
   
const String& VirtualClass::getName() const
{
   return mName;
}

void VirtualClass::setName(const String& name)
{
   mName = name;
}

bool VirtualClass::hasBaseName() const
{
   return !mBaseName.isEmpty();
}

const String& VirtualClass::getBaseName() const
{
   return mBaseName;
}

void VirtualClass::setBaseName(const String& name)
{
   mBaseName = name;
}

bool VirtualClass::hasBaseClass() const
{
   return mpBaseClass != NULL;
}

const VirtualClass& VirtualClass::getBaseClass() const
{
   return *mpBaseClass;
}
   
void VirtualClass::setBaseClass(const VirtualClass& baseclass)
{
   mpBaseClass = &baseclass;
}

const VirtualFunctionTable& VirtualClass::getVirtualFunctionTable() const
{
   return mVTable;
}

VirtualFunctionTable& VirtualClass::getVirtualFunctionTable()
{
   return mVTable;
}

const char* VirtualClass::getByteCode() const
{
   ASSERT_PTR(mpByteCode);
   return mpByteCode;
}

void VirtualClass::setByteCode(const char* pcode)
{
   mpByteCode = strdup(pcode);
}

int VirtualClass::getVariableCount() const
{
   return mVariableCount;
}
   
void VirtualClass::setVariableCount(int count)
{
   mVariableCount = count;
}

int VirtualClass::getStaticCount() const
{
   return mStaticCount;
}
   
void VirtualClass::setStaticCount(int count)
{
   ASSERT(mpStatics == NULL);

   mStaticCount = count;
   
   mpStatics = new Variant[mStaticCount];
}

const ASTClass& VirtualClass::getDefinition() const
{
   return *mpDefinition;
}

void VirtualClass::setDefinition(ASTClass* pdefinition)
{
   delete mpDefinition;
   mpDefinition = pdefinition;
}

VirtualObject& VirtualClass::getClassObject() const
{
   ASSERT_PTR(mpClassObject);
   return *mpClassObject;
}

void VirtualClass::setClassObject(VirtualObject* pobject)
{
   mpClassObject = pobject;
}

void VirtualClass::setFlags(Flags flags)
{
   mFlags = flags;
}

// - Query

bool VirtualClass::isNative() const
{
   return (mFlags & eNative) == eNative;
}

bool VirtualClass::canInstantiate() const
{
   return (mFlags & eInstantiatable) == eInstantiatable;
}

String VirtualClass::getNativeClassName() const
{
   if ( isNative() )
   {
      // we are only interested in the name of the class, not the package as well
      return mpDefinition->getName();
   }

   return hasBaseClass() ? getBaseClass().getNativeClassName() : String("");
}
   
bool VirtualClass::isBaseClass(const VirtualClass& base) const
{
   if ( this == &base )
   {
      return true;
   }
   else
   {
      if ( hasBaseClass() )
      {
         return ( &base == mpBaseClass ) ? true : getBaseClass().isBaseClass(base);
      }

      return false;
   }
}

bool VirtualClass::implements(const VirtualClass& interfce) const
{
   if ( mpDefinition->getInterfaces().contains(interfce.getDefinition()) )
   {
      return true;
   }
   else if ( hasBaseClass() )
   {
      return getBaseClass().implements(interfce);
   }
   return false;
}

const VirtualLookupTable& VirtualClass::getLookupTable(int index) const
{
   ASSERT(index >= 0 && index < mLookupTables.size());
   return *mLookupTables[index];
}

const VirtualFunctionTableEntry* VirtualClass::getDefaultConstructor() const
{
   String name = mName;
   int pos = mName.lastIndexOf('.');
   if ( pos != -1 )
   {
      name.remove(0, pos + 1);
   }

   return mVTable.findByName(name);
}

// - Operations

int VirtualClass::addLookupTable(VirtualLookupTable* ptable)
{
   mLookupTables.push_back(ptable);
   return mLookupTables.size() - 1;
}

void VirtualClass::offsetCode(int offset)
{
   for ( std::size_t index = 0; index < mLookupTables.size(); index++ )
   {
      mLookupTables[index]->offsetCode(offset);
   }
}

// - Runtime instantiation

void VirtualClass::instantiate(VirtualObject& object) const
{
   object.setClass(*this);
   object.initialize(mVariableCount);
}

const Variant& VirtualClass::getStatic(int index) const
{
   ASSERT_PTR(mpStatics);
   ASSERT(index >= 0);
   ASSERT(index < mStaticCount);

   return mpStatics[index];
}

Variant& VirtualClass::getStatic(int index)
{
   ASSERT_PTR(mpStatics);
   ASSERT(index >= 0);
   ASSERT(index < mStaticCount);

   return mpStatics[index];
}

void VirtualClass::setStatic(int index, const Variant& value)
{
   ASSERT_PTR(mpStatics);
   ASSERT(index >= 0);
   ASSERT(index < mStaticCount);

   mpStatics[index] = value;
}
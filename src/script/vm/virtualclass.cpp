
#include "virtualclass.h"

#include "core/defines.h"

#include "script/ast/astclass.h"
#include "script/common/variant.h"

#include "virtualobject.h"
#include "virtualarrayobject.h"
#include "virtualfunctiontable.h"
#include "virtuallookuptable.h"

VirtualClass::VirtualClass():
   mName(),
   mBaseName(),
   mpBaseClass(NULL),
   mpDefinition(NULL),
   mVTable(),
   mInstructions(),
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
   
const std::string& VirtualClass::getName() const
{
   return mName;
}

void VirtualClass::setName(const std::string& name)
{
   mName = name;
}

bool VirtualClass::hasBaseName() const
{
   return !mBaseName.empty();
}

const std::string& VirtualClass::getBaseName() const
{
   return mBaseName;
}

void VirtualClass::setBaseName(const std::string& name)
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

const VirtualInstructionTable& VirtualClass::getInstructions() const
{
   return mInstructions;
}

VirtualInstructionTable& VirtualClass::getInstructions()
{
   return mInstructions;
}

void VirtualClass::setInstructions(const VirtualInstructionTable& instructions)
{
   mInstructions = instructions;
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

std::string VirtualClass::getNativeClassName() const
{
   if ( isNative() )
   {
      // we are only interested in the name of the class, not the package as well
      return mpDefinition->getName();
   }

   return hasBaseClass() ? getBaseClass().getNativeClassName() : "";
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
   std::string name = mName;
   std::size_t pos = mName.find('.');
   if ( pos != std::string::npos )
   {
      name.erase(0, pos + 1);
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

VirtualObject* VirtualClass::instantiate() const
{
   VirtualObject* pobject = new VirtualObject();
   pobject->setClass(*this);
   pobject->initialize(mVariableCount);

   return pobject;
}

VirtualArrayObject* VirtualClass::instantiateArray() const
{
   VirtualArrayObject* pobject = new VirtualArrayObject();

   return pobject;
}

const Variant& VirtualClass::getStatic(int index) const
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
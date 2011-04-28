
#include "virtualclass.h"

#include "ast/astclass.h"

#include "common/variant.h"

#include "virtualclassobject.h"
#include "virtualarrayobject.h"
#include "virtualfunctiontable.h"

VirtualClass::VirtualClass():
   mName(),
   mBaseName(),
   mpBaseClass(NULL),
   mpDefinition(NULL),
   mVTable(),
   mInstructions(),
   mClassObject(NULL),
   mpStatics(NULL),
   mStaticCount(0),
   mVariableCount(0)
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
   BOOST_ASSERT(mpStatics == NULL);

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

const VirtualObjectReference& VirtualClass::getClassObject() const
{
   return mClassObject;
}

void VirtualClass::setClassObject(const VirtualObjectReference& object)
{
   mClassObject = object;
}

//  Query
   
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

// - Runtime instantiation

VirtualObject* VirtualClass::instantiate() const
{
   VirtualClassObject* pobject = new VirtualClassObject();
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
   BOOST_ASSERT(mpStatics != NULL);
   BOOST_ASSERT(index >= 0);
   BOOST_ASSERT(index < mStaticCount);

   return mpStatics[index];
}

void VirtualClass::setStatic(int index, const Variant& value)
{
   BOOST_ASSERT(mpStatics != NULL);
   BOOST_ASSERT(index >= 0);
   BOOST_ASSERT(index < mStaticCount);

   mpStatics[index] = value;
}
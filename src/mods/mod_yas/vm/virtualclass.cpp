
#include "virtualclass.h"

#include "core/defines.h"

#include "virtualobject.h"
#include "virtualfield.h"
#include "virtualfunction.h"
#include "virtualfunctiontable.h"
#include "virtualfunctiontableentry.h"
#include "virtuallookuptable.h"
#include "virtualvalue.h"

VirtualClass::VirtualClass():
   mName(),
   mBaseName(),
   mpBaseClass(NULL),
   mInterfaces(),
   mFields(),
   mStaticFields(),
   mFunctions(),
   mVTable(),
   mpClassObject(NULL),
   mStaticValues(),
   mpInterfaceLookupTable(NULL),
   mFlags(eNone)
{
}

VirtualClass::~VirtualClass()
{
   setInterfaceLookupTable(NULL);
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
   
void VirtualClass::setBaseClass(VirtualClass& baseclass)
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

const VirtualClass::Fields& VirtualClass::getFields() const
{
   return mFields;
}

VirtualClass::Fields& VirtualClass::getFields()
{
   return mFields;
}

const VirtualClass::Fields& VirtualClass::getStaticFields() const
{
   return mStaticFields;
}

VirtualClass::Fields& VirtualClass::getStaticFields()
{
   return mStaticFields;
}

int VirtualClass::getVariableCount() const
{
   return mFields.size();
}
   
int VirtualClass::getStaticCount() const
{
   return mStaticFields.size();
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

const int* VirtualClass::getInterfaceLookupTable() const
{
   return mpInterfaceLookupTable;
}
 
void VirtualClass::setInterfaceLookupTable(int* ptable)
{
   delete mpInterfaceLookupTable;
   mpInterfaceLookupTable = ptable;
}

void VirtualClass::setFlags(Flags flags)
{
   mFlags = flags;
}

VirtualClass::Functions& VirtualClass::getFunctions()
{
   return mFunctions;
}

// - Query

bool VirtualClass::isNative() const
{
   return (mFlags & eNative) == eNative;
}

bool VirtualClass::canInstantiate() const
{
   return true; // (mFlags & eInstantiatable) == eInstantiatable;
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
   if ( mInterfaces.contains(interfce) )
   {
      return true;
   }
   else if ( hasBaseClass() )
   {
      return getBaseClass().implements(interfce);
   }
   return false;
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

int VirtualClass::getTotalVariables() const
{
   return mFields.size() + (hasBaseClass() ? getBaseClass().getTotalVariables() : 0);
}

// - Operations

void VirtualClass::addField(VirtualField* pfield)
{
   pfield->setClass(*this);
   mFields.push_back(pfield);
}

void VirtualClass::addStaticField(VirtualField* pfield)
{
   pfield->setClass(*this);
   mStaticFields.push_back(pfield);
}

void VirtualClass::addFunction(VirtualFunction* pfunction)
{
   pfunction->setClass(*this);
   mFunctions.push_back(pfunction);
}

void VirtualClass::addInterface(VirtualClass& klass)
{
   mInterfaces.add(klass);
}

void VirtualClass::collectInterface(VirtualClasses& interfces)
{
   interfces.add(mInterfaces);
   if ( hasBaseClass() )
   {
      mpBaseClass->collectInterface(interfces);
   }
}

void VirtualClass::build()
{
   buildVariables();
   buildVirtualTable();
   buildInterfaceTable();
}

void VirtualClass::buildVariables()
{
   // variable index adds up with the base class
   int base = hasBaseClass() ? getBaseClass().getTotalVariables() : 0;
   for ( int index = 0; index < mFields.size(); index++ )
   {
      VirtualField* pfield = mFields[index];
      pfield->setIndex(base + index);
   }

   // statics are class specific, so only use index
   for ( int index = 0; index < mStaticFields.size(); index++ )
   {
      VirtualField* pfield = mStaticFields[index];
      pfield->setIndex(index);
   }

   mStaticValues.resize(mStaticFields.size());
}

void VirtualClass::buildVirtualTable()
{
   if ( hasBaseClass() )
   {
      mVTable = mpBaseClass->getVirtualFunctionTable();
   }

   // the static_init & var_init always are at 0 and 1 respectively
   mVTable.setInits(*mFunctions[0], *mFunctions[1]);

   // now add the other functions to the v-table
   for ( std::size_t index = 2; index < mFunctions.size(); ++index )
   {
      VirtualFunction* pfunction = mFunctions[index];
      int vpos = mVTable.insert(*pfunction);
      pfunction->setIndex(vpos);
   }
}

void VirtualClass::buildInterfaceTable()
{
   int max = 0;

   VirtualClasses interfaces;
   collectInterface(interfaces);

   // determine the highest number of used function
   for ( int index = 0; index < interfaces.size(); ++index )
   {
      const VirtualClass& interfce = interfaces[index];
      const VirtualFunctionTable& table = interfce.getVirtualFunctionTable();
   
      if ( table.size() > max )
      {
         max = table.size();
      }
   }

   if ( max > 0 )
   {
      // create the lookup table (each interface function is at the same position in the table
      // and links to the actual function number in the virtual table.

      mpInterfaceLookupTable = new int[max + 1];
      for ( int index = 0; index < interfaces.size(); ++index )
      {
         const VirtualClass& iface = interfaces[index];
         const VirtualFunctionTable& table = iface.getVirtualFunctionTable();
      
         for ( int entry = 0; entry < table.size(); ++entry )
         {
            const VirtualFunction& func = *table[entry].mpFunction;
            const VirtualFunction* plookupfunc = findExactMatch(func);
            if ( plookupfunc == NULL )
            {
               throw std::exception("Invalid class!");
            }

            ASSERT(func.getIndex() <= max);
            mpInterfaceLookupTable[func.getIndex()] = plookupfunc->getIndex();
         }
      }
   }
}

// - Runtime instantiation

void VirtualClass::instantiate(VirtualObject& object) const
{
   object.setClass(*this);
   object.initialize(getTotalVariables());
}

const VirtualValue& VirtualClass::getStatic(int index) const
{
   ASSERT(index >= 0);
   ASSERT(index < mStaticValues.size());

   return mStaticValues[index];
}

VirtualValue& VirtualClass::getStatic(int index)
{
   ASSERT(index >= 0);
   ASSERT(index < mStaticValues.size());

   return mStaticValues[index];
}

void VirtualClass::setStatic(int index, const VirtualValue& value)
{
   ASSERT(index >= 0);
   ASSERT(index < mStaticFields.size());

   mStaticValues[index] = value;
}

// - Searching

VirtualFunction* VirtualClass::findExactMatch(const String& name, const yasc::Types& args)
{
   for ( std::size_t index = 0; index < mFunctions.size(); ++index )
   {
      VirtualFunction* pfunction = mFunctions[index];
      if ( pfunction->getName() == name && pfunction->getArguments().equals(args) )
      {
         return pfunction;
      }
   }
   return hasBaseClass() ? mpBaseClass->findExactMatch(name, args) : NULL;
}

VirtualFunction* VirtualClass::findExactMatch(const VirtualFunction& function)
{
   for ( std::size_t index = 0; index < mFunctions.size(); ++index )
   {
      VirtualFunction* pfunc = mFunctions[index];
      if ( pfunc->equals(function) )
      {
         return pfunc;
      }
   }
   return hasBaseClass() ? mpBaseClass->findExactMatch(function) : NULL;
}

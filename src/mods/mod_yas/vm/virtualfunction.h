
#ifndef VIRTUAL_FUNCTION_H
#define VIRTUAL_FUNCTION_H

#include "core/string/string.h"

#include "mod_yas/common/modifiers.h"
#include "mod_yas/common/types.h"

#include "virtualguards.h"
#include "virtuallookuptables.h"

#include "../cil/instructions.h"

class VirtualClass;
class VirtualValue;

class VirtualFunction
{
public:
   VirtualFunction();

 // get/set
   const String& getName() const;
   void          setName(const String& name);

   const VirtualClass& getClass() const;
   void                setClass(VirtualClass& klass);

   const Modifiers& getModifiers() const;
   void             setModifiers(const Modifiers& modifiers);

   int  getFirstInstruction() const;
   void setFirstInstruction(int first);

   const yasc::Types& getArguments() const;
   void               setArguments(const yasc::Types& args);

   const yasc::Types& getLocals() const;
   void               setLocals(const yasc::Types& args);

   const yasc::Type& getReturnType() const;
   void              setReturnType(const yasc::Type& type);

   const CIL::Instructions& getInstructions() const;
   void                     setInstructions(const CIL::Instructions& insts);

   VirtualGuards& getGuards();

   const VirtualLookupTables& getLookupTables() const;
         VirtualLookupTables& getLookupTables();

   int  getIndex() const;
   void setIndex(int index);

 // query
   bool equals(const VirtualFunction& that) const;
   int lookup(int tableid, const VirtualValue& value) const;
   
 // operations
   void addLookupTable(VirtualLookupTable* ptable);
   void updateLookupTables();

   void addGuard(VirtualGuard* pguard);
   void updateGuards();

 // search
   const VirtualGuard* findGuard(int ip) const;

private:
   
 // data
   String               mName;
   VirtualClass*        mpClass;
   Modifiers            mModifiers;
   yasc::Types          mArguments;
   yasc::Types          mLocals;
   yasc::Type*          mpReturnType;
   VirtualGuards        mGuards;
   VirtualLookupTables  mLookups;
   CIL::Instructions    mInstructions;
   int                  mIndex;
   int                  mFirstInstruction;
};

#endif // VIRTUAL_FUNCTION_H

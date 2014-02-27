
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
   ~VirtualFunction();

 // get/set
   const String& getName() const;
   void          setName(const String& name);

   const VirtualClass& getClass() const;
   void                setClass(VirtualClass& klass);

   const Modifiers& getModifiers() const;
   void             setModifiers(const Modifiers& modifiers);

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

   bool        hasCode() const;
   const char* getCode();
   void        setCode(char* pcode, int len);

   int  getIndex() const;
   void setIndex(int index);

 // query
   bool equals(const VirtualFunction& that) const;
   int lookup(int tableid, const VirtualValue& value) const;
   int getArgumentCount() const;
   
 // operations
   void addLookupTable(VirtualLookupTable* ptable);
   void addGuard(VirtualGuard* pguard);

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
   char*                mpCode;
   int                  mCodeLength;
   int                  mIndex;
};

#endif // VIRTUAL_FUNCTION_H

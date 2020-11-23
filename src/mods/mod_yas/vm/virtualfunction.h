
#ifndef VIRTUAL_FUNCTION_H
#define VIRTUAL_FUNCTION_H

#include <libcil/cil/instructions.h>
#include <libcil/common/modifiers.h>
#include <libcil/common/types.h>
#include <libcil/common/type.h>

#include "core/string/string.h"
#include "core/string/stringlist.h"

#include "virtualguards.h"
#include "virtuallookuptables.h"


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

   const CIL::Types& getArguments() const;
   void               setArguments(const CIL::Types& args);

   const CIL::Types& getLocals() const;
   void               setLocals(const CIL::Types& args);

   const CIL::Type& getReturnType() const;
   void              setReturnType(const CIL::Type& type);

   const StringList& getAnnotations() const;

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
   void addAnnotation(const String& annotation);
   void addLookupTable(VirtualLookupTable* ptable);
   void addGuard(const VirtualGuard& guard);

 // search
   const VirtualGuard* findGuard(int ip) const;

private:
   
 // data
   String               mName;
   VirtualClass*        mpClass;
   Modifiers            mModifiers;
   CIL::Types          mArguments;
   CIL::Types          mLocals;
   CIL::Type           mReturnType;
   VirtualGuards        mGuards;
   VirtualLookupTables  mLookups;
   StringList           mAnnotations;
   CIL::Instructions    mInstructions;
   char*                mpCode;
   int                  mCodeLength;
   int                  mIndex;
};

#endif // VIRTUAL_FUNCTION_H

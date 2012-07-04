
#ifndef VIRTUAL_CLASS_H_
#define VIRTUAL_CLASS_H_

#include "script/script_base.h"

#include <string>

#include "virtualinstructiontable.h"
#include "virtualfunctiontable.h"

class ASTClass;
class Variant;
class VirtualArrayObject;
class VirtualLookupTable;
class VirtualObject;

class SCRIPT_API VirtualClass
{
   typedef std::vector<VirtualLookupTable*> LookupTables;

public:
   enum Flags {
      eNone = 0, 
      eInstantiatable = 1,
      eNative = 2
   };

   VirtualClass();
   ~VirtualClass();

 // get/set
   const std::string& getName() const;
   void               setName(const std::string& name);

   bool               hasBaseName() const;
   const std::string& getBaseName() const;
   void               setBaseName(const std::string& name);

   bool                hasBaseClass() const;
   const VirtualClass& getBaseClass() const;
   void                setBaseClass(const VirtualClass& pbaseclass);

   const VirtualFunctionTable& getVirtualFunctionTable() const;
         VirtualFunctionTable& getVirtualFunctionTable();

   const VirtualInstructionTable& getInstructions() const;
         VirtualInstructionTable& getInstructions();
   void                           setInstructions(const VirtualInstructionTable& instructions);

   const ASTClass& getDefinition() const;
   void            setDefinition(ASTClass* pdefinition);

   int  getVariableCount() const;
   void setVariableCount(int count);

   int  getStaticCount() const;
   void setStaticCount(int count);

   VirtualObject& getClassObject() const;
   void           setClassObject(VirtualObject* pobject);

   void setFlags(Flags flags);

 // query
   bool isNative() const;
   bool canInstantiate() const;

   std::string getNativeClassName() const;

   bool isBaseClass(const VirtualClass& base) const;
   bool implements(const VirtualClass& interfce) const;

   const VirtualLookupTable& getLookupTable(int index) const;
   const VirtualFunctionTableEntry* getDefaultConstructor() const;

 // operations
   VirtualObject*      instantiate() const;
   VirtualArrayObject* instantiateArray() const;

   const Variant& getStatic(int index) const;
   void           setStatic(int index, const Variant& value);

   int addLookupTable(VirtualLookupTable* ptable);

   void offsetCode(int offset);
   
private:
   std::string             mName;
   std::string             mBaseName;
   const VirtualClass*     mpBaseClass;
   ASTClass*               mpDefinition;
   VirtualFunctionTable    mVTable;
   VirtualInstructionTable mInstructions;
   VirtualObject*          mpClassObject;
   LookupTables            mLookupTables;
   Variant*                mpStatics;
   int                     mStaticCount;
   int                     mVariableCount;
   Flags                   mFlags;
};

#endif // VIRTUAL_CLASS_H_

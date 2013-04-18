
#ifndef VIRTUAL_CLASS_H_
#define VIRTUAL_CLASS_H_

#include "script/script_base.h"

#include "core/string/string.h"

#include "virtualinstructiontable.h"
#include "virtualfunctiontable.h"

class ASTClass;
class Variant;
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
   const String& getName() const;
   void               setName(const String& name);

   bool               hasBaseName() const;
   const String& getBaseName() const;
   void               setBaseName(const String& name);

   bool                hasBaseClass() const;
   const VirtualClass& getBaseClass() const;
   void                setBaseClass(const VirtualClass& pbaseclass);

   const VirtualFunctionTable& getVirtualFunctionTable() const;
         VirtualFunctionTable& getVirtualFunctionTable();

   const char* getByteCode() const;
   void        setByteCode(const char* pbytecode);

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

   String getNativeClassName() const;

   bool isBaseClass(const VirtualClass& base) const;
   bool implements(const VirtualClass& interfce) const;

   const VirtualLookupTable& getLookupTable(int index) const;
   const VirtualFunctionTableEntry* getDefaultConstructor() const;

 // operations
   void instantiate(VirtualObject& object) const;

   const Variant& getStatic(int index) const;
         Variant& getStatic(int index);
   void           setStatic(int index, const Variant& value);

   int addLookupTable(VirtualLookupTable* ptable);

   void offsetCode(int offset);
   
private:
   String                  mName;
   String                  mBaseName;
   const VirtualClass*     mpBaseClass;
   ASTClass*               mpDefinition;
   VirtualFunctionTable    mVTable;
   char*                   mpByteCode;
   VirtualObject*          mpClassObject;
   LookupTables            mLookupTables;
   Variant*                mpStatics;
   int                     mStaticCount;
   int                     mVariableCount;
   Flags                   mFlags;
};

#endif // VIRTUAL_CLASS_H_


#ifndef VIRTUAL_CLASS_H_
#define VIRTUAL_CLASS_H_

#include <string>

#include "virtualinstructiontable.h"
#include "virtualfunctiontable.h"
#include "virtualobjectreference.h"

class ASTClass;
class Variant;
class VirtualArrayObject;

class VirtualClass
{
public:
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

   const VirtualObjectReference& getClassObject() const;
   void setClassObject(const VirtualObjectReference& object);

 // query
   bool isBaseClass(const VirtualClass& base) const;
   
 // operations
   VirtualObject*      instantiate() const;
   VirtualArrayObject* instantiateArray() const;

   const Variant& getStatic(int index) const;
   void           setStatic(int index, const Variant& value);
   
private:
   std::string             mName;
   std::string             mBaseName;
   const VirtualClass*     mpBaseClass;
   ASTClass*               mpDefinition;
   VirtualFunctionTable    mVTable;
   VirtualInstructionTable mInstructions;
   VirtualObjectReference  mClassObject;
   Variant*                mpStatics;
   int                     mStaticCount;
   int                     mVariableCount;
};

#endif // VIRTUAL_CLASS_H_

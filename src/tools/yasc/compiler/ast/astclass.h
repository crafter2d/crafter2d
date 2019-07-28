
#ifndef AST_CLASS_H_
#define AST_CLASS_H_

#include <map>
#include <vector>

#include "core/string/string.h"

#include "yasc/compiler/classresolver.h"
   
#include "astnode.h"
#include "astmodifier.h"
#include "asttypelist.h"
#include "astfunctionmap.h"
#include "astfunctiontable.h"

namespace CIL
{
   class Class;
};

class ASTMember;
class ASTFunction;
class ASTField;
class ASTType;
class ASTTypeList;
class ASTTypeVariables;
class ASTSignature;
class Scope;
class CompileContext;

class ASTClass : public ASTNode
{
public:
   enum SearchScope { eLocal, eAll };
   enum Kind { eClass, eInterface };

   typedef std::vector<ASTField*> Fields;

   ASTClass(); 
   virtual ~ASTClass();

 // get/set
   Kind getKind() const;
   void setKind(Kind kind);

   bool           hasBaseType() const;
   const ASTType& getBaseType() const;
         ASTType& getBaseType();
   void           setBaseType(ASTType* pbasetype);

   bool            hasBaseClass() const;
   const ASTClass& getBaseClass() const;
         ASTClass& getBaseClass();
   void            setBaseClass(ASTClass& baseclass);

   const String& getName() const;
   void          setName(const String& name);

   const String& getFullName() const;
   void          setFullName(const String& name);

   const ASTModifiers& getModifiers() const;
         ASTModifiers& getModifiers();

   const ASTTypeVariables& getTypeVariables() const;
         ASTTypeVariables& getTypeVariables();
   void                    setTypeVariables(ASTTypeVariables* ptypes);

   const ASTFunctionMap& getFunctions() const;
         ASTFunctionMap& getFunctions();

   const ASTFunctionTable& getFunctionTable() const;
         ASTFunctionTable& getFunctionTable();

   const ASTTypeList& getInterfaces() const;
         ASTTypeList& getInterfaces();

   const Fields& getStatics() const;
   const Fields& getFields() const;
      
   CIL::Class& getCompiledClass();
   CIL::Class* useCompiledClass();
   void        setCompiledClass(CIL::Class* pclass);

 // query
   bool isClass() const;
   bool isInterface() const;

   bool isBase(const ASTClass& base) const;
   bool isImplementing(const ASTClass& intrface) const;
   bool isLocal(const ASTFunction& function) const;
   bool isMember(const String& name) const;
   bool isNative() const;

   bool isGeneric() const;
   bool isTypeName(const String& name) const;

   bool hasConstructor() const;
   bool hasAbstractFunction() const;
   bool hasNativeFunction() const;
   bool hasNativeConstructor() const;

   int getTotalStatics() const;
   int getTotalVariables() const;

   const ASTField& getField(int index) const;

 // operations
   void addInterface(ASTType* pinterfacetype);
   void addMember(ASTMember* pmember);
   void insertFunction(int index, ASTFunction* pfunction);

   void ensureDefaultConstructor();

   ASTType* createThisType() const;

   void collectInterfaces(ASTTypeList& interfaces) const;

   void registerVariables(Scope& scope) const;

   void calculateResources();

 // search
   const ASTField* findStatic(const String& name, SearchScope scope = eAll) const;
         ASTField* findStatic(const String& name, SearchScope scope = eAll);

   const ASTField* findField(const String& name, SearchScope scope = eAll) const;
         ASTField* findField(const String& name, SearchScope scope = eAll);

   const ASTFunction* findBestMatch(const String& name, const ASTSignature& signature, const ASTTypeList& types) const;
         ASTFunction* findBestMatch(const String& name, const ASTSignature& signature, const ASTTypeList& types);

   const ASTFunction* findExactMatch(const String& name, const ASTSignature& signature) const;
         ASTFunction* findExactMatch(const String& name, const ASTSignature& signature);

 // visitor
   ACCEPT;

private:
 // operations
   void indexStatics();
   void indexVariables();
   void indexFunctions();

 // search
   ASTFunction* findExactMatchLocal(const String& name, const ASTSignature& signature);

   Kind              mKind;
   ASTModifiers      mModifiers;
   ASTType*          mpBaseType;
   ASTTypeList       mInterfaces;
   ASTTypeVariables* mpTypeVariables;
   ASTFunctionMap    mFunctions;
   ASTFunctionTable  mFunctionTable;
   String            mName;
   String            mFullName;
   Fields            mStatics;
   Fields            mFields;
   CIL::Class*       mpCompiledClass;
};

#endif // AST_CLASS_H_


#ifndef AST_CLASS_H_
#define AST_CLASS_H_

#include <map>
#include <string>
#include <vector>

#include "script/compiler/functiontable.h"

#include "script/common/literaltable.h"

#include "astnode.h"
#include "astmodifier.h"
#include "asttypelist.h"

class ASTMember;
class ASTFunction;
class ASTField;
class ASTType;
class ASTTypeList;
class ASTTypeVariables;
class Scope;
class Signature;
class CompileContext;

class ASTClass : public ASTNode
{
   typedef std::multimap<std::string, ASTFunction*> Functions;

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

   const std::string& getName() const;
   void               setName(const std::string& name);

   const ASTModifiers& getModifiers() const;
         ASTModifiers& getModifiers();

   const ASTTypeVariables& getTypeVariables() const;
         ASTTypeVariables& getTypeVariables();
   void                    setTypeVariables(ASTTypeVariables* ptypes);

   const FunctionTable& getFunctionTable() const;
         FunctionTable& getFunctionTable();

   const ASTTypeList& getInterfaces() const;
         ASTTypeList& getInterfaces();

   const Fields& getStatics() const;
   const Fields& getFields() const;

 // query
   bool isBase(const ASTClass& base) const;
   bool isImplementing(const ASTClass& intrface) const;
   bool isLocal(const ASTFunction& function) const;
   bool isGeneric() const;

   bool hasConstructor() const;
   bool hasAbstractFunction() const;

   int getTotalStatics() const;
   int getTotalVariables() const;

 // operations
   void addInterface(ASTType* pinterfacetype);
   void addMember(ASTMember* pmember);

   void registerVariables(Scope& scope) const;

   void calculateResources();

 // search
   const ASTField* findStatic(const std::string& name, SearchScope scope = eAll) const;
         ASTField* findStatic(const std::string& name, SearchScope scope = eAll);

   const ASTField* findField(const std::string& name, SearchScope scope = eAll) const;
         ASTField* findField(const std::string& name, SearchScope scope = eAll);

   const ASTFunction* findBestMatch(const std::string& name, const Signature& signature, const ASTTypeList& types) const;
         ASTFunction* findBestMatch(const std::string& name, const Signature& signature, const ASTTypeList& types);

   const ASTFunction* findExactMatch(const std::string& name, const Signature& signature) const;
         ASTFunction* findExactMatch(const std::string& name, const Signature& signature);

   const ASTFunction* findInterfaceFunction(const ASTFunction& function) const;

 // visitor
   ACCEPT;

private:
 // operations
   void indexStatics();
   void indexVariables();
   void indexFunctions();

 // search
   ASTFunction* findExactMatchLocal(const std::string& name, const Signature& signature);

   Kind              mKind;
   ASTModifiers      mModifiers;
   ASTType*          mpBaseType;
   ASTTypeList       mInterfaces;
   std::string       mName;
   ASTTypeVariables* mpTypeVariables;
   FunctionTable     mTable;
   Fields            mStatics;
   Fields            mFields;
   Functions         mFunctions;
};

#endif // AST_CLASS_H_

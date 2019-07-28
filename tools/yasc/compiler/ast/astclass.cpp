
#include "astclass.h"

#include "core/defines.h"

#include "yasc/compiler/scope/scope.h"
#include "yasc/compiler/scope/scopevariable.h"
#include "yasc/cil/class.h"

#include "astfunction.h"
#include "asttype.h"
#include "asttypevariables.h"
#include "astvariable.h"
#include "astfield.h"
#include "astvisitor.h"

ASTClass::ASTClass():
   ASTNode(),
   mKind(eClass),
   mModifiers(),
   mpBaseType(nullptr),
   mInterfaces(),
   mpTypeVariables(nullptr),
   mFunctions(),
   mFunctionTable(),
   mName(),
   mFullName(),
   mStatics(),
   mFields(),
   mpCompiledClass(nullptr)
{
}

ASTClass::~ASTClass()
{
   setBaseType(nullptr);
   setTypeVariables(nullptr);
   setCompiledClass(nullptr);
}

// - Get/set

ASTClass::Kind ASTClass::getKind() const
{
   return mKind;
}

void ASTClass::setKind(Kind kind)
{
   mKind = kind;
}

bool ASTClass::hasBaseType() const
{
   return mpBaseType != nullptr;
}
   
const ASTType& ASTClass::getBaseType() const
{
   return *mpBaseType;
}

ASTType& ASTClass::getBaseType()
{
   return *mpBaseType;
}

void ASTClass::setBaseType(ASTType* pbasetype)
{
   delete mpBaseType;
   mpBaseType = pbasetype;
}

bool ASTClass::hasBaseClass() const
{
   return hasBaseType() && !getBaseType().isUnknown();
}
   
const ASTClass& ASTClass::getBaseClass() const
{
   ASSERT_PTR(mpBaseType);
   ASSERT(mpBaseType->hasObjectClass());
   return mpBaseType->getObjectClass();
}

ASTClass& ASTClass::getBaseClass()
{
   ASSERT_PTR(mpBaseType);
   return mpBaseType->getObjectClass();
}

void ASTClass::setBaseClass(ASTClass& baseclass)
{
   ASSERT_PTR(mpBaseType);
   mpBaseType->setObjectClass(baseclass);
}

const String& ASTClass::getName() const
{
   return mName;
}
   
void ASTClass::setName(const String& name)
{
   mName = name;
}

const String& ASTClass::getFullName() const
{
   return mFullName;
}

void ASTClass::setFullName(const String& name)
{
   mFullName = name;
}

const ASTModifiers& ASTClass::getModifiers() const
{
   return mModifiers;
}

ASTModifiers& ASTClass::getModifiers()
{
   return mModifiers;
}

const ASTTypeVariables& ASTClass::getTypeVariables() const
{
   ASSERT_PTR(mpTypeVariables);
   return *mpTypeVariables;
}

ASTTypeVariables& ASTClass::getTypeVariables()
{
   ASSERT_PTR(mpTypeVariables);
   return *mpTypeVariables;
}

void ASTClass::setTypeVariables(ASTTypeVariables* pinfo)
{
   delete mpTypeVariables;
   mpTypeVariables = pinfo;
}

const ASTFunctionMap& ASTClass::getFunctions() const
{
   return mFunctions;
}

ASTFunctionMap& ASTClass::getFunctions()
{
   return mFunctions;
}

const ASTFunctionTable& ASTClass::getFunctionTable() const
{
   return mFunctionTable;
}

ASTFunctionTable& ASTClass::getFunctionTable()
{
   return mFunctionTable;
}

const ASTTypeList& ASTClass::getInterfaces() const
{
   return mInterfaces;
}
 
ASTTypeList& ASTClass::getInterfaces()
{
   return mInterfaces;
}

const ASTClass::Fields& ASTClass::getStatics() const
{
   return mStatics;
}

const ASTClass::Fields& ASTClass::getFields() const
{
   return mFields;
}

CIL::Class& ASTClass::getCompiledClass()
{
   ASSERT_PTR(mpCompiledClass);
   return *mpCompiledClass;
}

CIL::Class* ASTClass::useCompiledClass()
{
   CIL::Class* presult = mpCompiledClass;
   mpCompiledClass = nullptr;
   return presult;
}

void ASTClass::setCompiledClass(CIL::Class* pclass)
{
   delete mpCompiledClass;
   mpCompiledClass = pclass;
}

// - Query

bool ASTClass::isClass() const
{
   return mKind == eClass;
}
   
bool ASTClass::isInterface() const
{
   return mKind == eInterface;
}
   
bool ASTClass::isBase(const ASTClass& base) const
{
   if ( this == &base )
   {
      return true;
   }
   else if ( hasBaseClass() )
   {
      const ASTClass& baseclass = getBaseClass();

      if ( &baseclass == &base )
      {
         return true;
      }
      else
      {
         return baseclass.isBase(base);
      }
   }
   return false;
}

bool ASTClass::isImplementing(const ASTClass& intrface) const
{
   if ( mInterfaces.contains(intrface) )
   {
      return true;
   }
   else if ( hasBaseClass() )
   {
      return getBaseClass().isImplementing(intrface);
   }

   return false;
}

bool ASTClass::isLocal(const ASTFunction& function) const
{
   return &function.getClass() == this;
}

bool ASTClass::isNative() const
{
   return mModifiers.isNative() || hasNativeFunction();
}

bool ASTClass::isGeneric() const
{
   return mpTypeVariables != nullptr;
}

bool ASTClass::isTypeName(const String& name) const
{
   return isGeneric() && mpTypeVariables->contains(name);
}

bool ASTClass::hasConstructor() const
{
   return mFunctions.hasConstructor();
}

bool ASTClass::hasAbstractFunction() const
{
   return mFunctions.hasAbstractFunction();
}

bool ASTClass::hasNativeFunction() const
{
   return mFunctions.hasNativeFunction();
}

bool ASTClass::hasNativeConstructor() const
{
   return mFunctions.hasNativeConstructor();
}

int ASTClass::getTotalStatics() const
{
   return mStatics.size();
}

int ASTClass::getTotalVariables() const
{
   return mFields.size() + (hasBaseClass() ? getBaseClass().getTotalVariables() : 0);
}

const ASTField& ASTClass::getField(int var) const
{
   if ( mFields.empty() || mFields[0]->getVariable().getResourceIndex() > var )
   {
      return getBaseClass().getField(var);
   }

   std::size_t index = var - mFields[0]->getVariable().getResourceIndex();
   ASSERT(index < mFields.size());
   return *mFields[index];
}

// - Operations
   
void ASTClass::addInterface(ASTType* ptype)
{
   mInterfaces.append(ptype);
}

bool ASTClass::isMember(const String& name) const
{
   const ASTMember* pmember = findField(name);
   if ( pmember == nullptr )
   {
      pmember = findStatic(name);
      if ( pmember == nullptr )
      {
         return mFunctions.hasFunction(name);
      }
   }

   return true;
}

void ASTClass::addMember(ASTMember* pmember)
{
   if ( pmember->getKind() == ASTMember::eField )
   {
      ASTField* pfield = dynamic_cast<ASTField*>(pmember);

      if ( pfield->getVariable().getModifiers().isStatic() )
      {
         mStatics.push_back(pfield);
      }
      else
      {
         mFields.push_back(pfield);
      }
   }
   else
   {
      ASTFunction* pfunction = dynamic_cast<ASTFunction*>(pmember);
      pfunction->setClass(*this);

      mFunctions.insert(pfunction);
   }

   pmember->setClass(*this);
   addChild(pmember);
}

void ASTClass::insertFunction(int index, ASTFunction* pfunction)
{
   mFunctions.insert(pfunction);
   mFunctionTable.set(index, *pfunction);

   insertChild(index, pfunction);
}

void ASTClass::ensureDefaultConstructor()
{
   if ( isClass() && !hasConstructor() )
   {
      ASTFunction* pconstructor = new ASTFunction(ASTMember::eConstructor);
      pconstructor->setName(mName);
      pconstructor->setType(new ASTType(ASTType::eVoid));
      pconstructor->getModifiers().setVisibility(ASTModifiers::ePublic);

      addMember(pconstructor);
   }
}

ASTType* ASTClass::createThisType() const
{
   ASTType* presult = new ASTType(ASTType::eObject);
   presult->setObjectName(getFullName());
   presult->setObjectClass(const_cast<ASTClass&>(*this));
   return presult;
}

void ASTClass::collectInterfaces(ASTTypeList& interfaces) const
{
   interfaces.append(mInterfaces);

   if ( hasBaseClass() )
   {
      getBaseClass().collectInterfaces(interfaces);
   }
}

void ASTClass::registerVariables(Scope& scope) const
{
   if ( hasBaseClass() )
   {
      getBaseClass().registerVariables(scope);
   }

   for ( std::size_t index = 0; index < mStatics.size(); index++ )
   {
      ASTField* pstatic = mStatics[index];
      scope.add(ScopeVariable::fromVariable(pstatic->getVariable()));
   }

   for ( std::size_t index = 0; index < mFields.size(); index++ )
   {
      ASTField* pvariable = mFields[index];
      scope.add(ScopeVariable::fromVariable(pvariable->getVariable()));
   }
}

void ASTClass::calculateResources()
{
   indexStatics();
   indexVariables();
   indexFunctions();
}

void ASTClass::indexStatics()
{
   for ( std::size_t index = 0; index < mStatics.size(); index++ )
   {
      ASTField* pstatic = mStatics[index];
      pstatic->getVariable().setResourceIndex(index);
   }
}

void ASTClass::indexVariables()
{
   int base = hasBaseClass() ? getBaseClass().getTotalVariables() : 0;

   for ( std::size_t index = 0; index < mFields.size(); index++ )
   {
      ASTField* pvariable = mFields[index];
      pvariable->getVariable().setResourceIndex(base + index);
   }
}

void ASTClass::indexFunctions()
{
   if ( isClass() )
   {
      if ( hasBaseClass() )
      {
         mFunctionTable = getBaseClass().getFunctionTable();
      }
   }

   mFunctionTable.build(mFunctions);
}

// - Search

const ASTField* ASTClass::findStatic(const String& name, SearchScope scope) const
{
   return const_cast<ASTClass&>(*this).findStatic(name, scope);
}

ASTField* ASTClass::findStatic(const String& name, SearchScope scope)
{
   for ( std::size_t index = 0; index < mStatics.size(); index++ )
   {
      ASTField* pstatic = mStatics[index];
      if ( pstatic->getVariable().getName() == name )
      {
         return pstatic;
      }
   }
   return scope == eAll && hasBaseClass() ? getBaseClass().findStatic(name, eAll) : nullptr;
}

const ASTField* ASTClass::findField(const String& name, SearchScope scope) const
{
   return const_cast<ASTClass&>(*this).findField(name, scope);
}

ASTField* ASTClass::findField(const String& name, SearchScope scope)
{
   for ( std::size_t index = 0; index < mFields.size(); index++ )
   {
      ASTField* pvariable = mFields[index];
      if ( pvariable->getVariable().getName() == name )
      {
         return pvariable;
      }
   }
   return scope == eAll && hasBaseClass() ? getBaseClass().findField(name) : nullptr;
}

const ASTFunction* ASTClass::findBestMatch(const String& name, const ASTSignature& signature, const ASTTypeList& types) const
{
   return const_cast<ASTClass&>(*this).findBestMatch(name, signature, types);
}

ASTFunction* ASTClass::findBestMatch(const String& name, const ASTSignature& signature, const ASTTypeList& types)
{
   ASTFunction* pfunction = mFunctions.findBestMatch(name, signature, types);
   if ( pfunction == nullptr && hasBaseClass() )
   {
      pfunction = getBaseClass().findBestMatch(name, signature, types);
   }
   return pfunction;
}

const ASTFunction* ASTClass::findExactMatch(const String& name, const ASTSignature& signature) const
{
   return const_cast<ASTClass&>(*this).findExactMatch(name, signature);
}

ASTFunction* ASTClass::findExactMatch(const String& name, const ASTSignature& signature)
{
   ASTFunction* pfunction = mFunctions.findExactMatch(name, signature);
   if ( pfunction == nullptr && hasBaseClass() )
   {
      pfunction = getBaseClass().findExactMatch(name, signature);
   }
   return pfunction;
}

// - Visitor
   
ACCEPT_IMPL(ASTClass)
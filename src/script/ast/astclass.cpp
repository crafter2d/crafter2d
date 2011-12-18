
#include "astclass.h"

#include "core/defines.h"

#include "script/scope/scope.h"
#include "script/scope/scopevariable.h"
#include "script/compiler/signature.h"

#include "astfunction.h"
#include "asttype.h"
#include "asttypevariables.h"
#include "astvariable.h"
#include "astfield.h"
#include "astvisitor.h"

ASTClass::ASTClass():
   ASTNode(),
   mKind(eClass),
   mResolver(),
   mModifiers(),
   mpBaseType(NULL),
   mInterfaces(),
   mName(),
   mFullName(),
   mpTypeVariables(NULL),
   mTable(),
   mStatics(),
   mFields(),
   mFunctions(),
   mState(eParsed)
{
}

ASTClass::~ASTClass()
{
   setBaseType(NULL);
   setTypeVariables(NULL);
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
   return mpBaseType != NULL;
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
   return hasBaseType();
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

const std::string& ASTClass::getName() const
{
   return mName;
}
   
void ASTClass::setName(const std::string& name)
{
   mName = name;
}

const std::string& ASTClass::getFullName() const
{
   return mFullName;
}

void ASTClass::setFullName(const std::string& name)
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

const FunctionTable& ASTClass::getFunctionTable() const
{
   return mTable;
}

FunctionTable& ASTClass::getFunctionTable()
{
   if ( mTable.size() == 0 )
   {
      indexFunctions();
   }

   return mTable;
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

ASTClass::State ASTClass::getState() const
{
   return mState;
}

void ASTClass::setState(State state) const
{
   mState = state;
}

// - Query
   
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

bool ASTClass::isGeneric() const
{
   return mpTypeVariables != NULL;
}

bool ASTClass::hasConstructor() const
{
   Functions::const_iterator it = mFunctions.begin();
   for ( ; it != mFunctions.end(); it++ )
   {
      ASTFunction* pfunction = it->second;
      if ( pfunction->getKind() == ASTMember::eConstructor )
      {
         return true;
      }
   }
   return false;
}

bool ASTClass::hasAbstractFunction() const
{
   Functions::const_iterator it = mFunctions.begin();
   for ( ; it != mFunctions.end(); it++ )
   {
      ASTFunction* pfunction = it->second;
      if ( pfunction->getModifiers().isAbstract() )
      {
         return true;
      }
   }
   return false;
}

int ASTClass::getTotalStatics() const
{
   return mStatics.size();
}

int ASTClass::getTotalVariables() const
{
   return mFields.size() + (hasBaseClass() ? getBaseClass().getTotalVariables() : 0);
}

// - Operations
   
void ASTClass::addInterface(ASTType* ptype)
{
   mInterfaces.append(ptype);
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

      mFunctions.insert(make_pair(pfunction->getName(), pfunction));
   }

   addChild(pmember);
}

const ClassResolver& ASTClass::getResolver() const
{
   return mResolver;
}

void ASTClass::setResolver(const ClassResolver& resolver)
{
   mResolver = resolver;
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
   int index = 0;

   if ( hasBaseClass() )
   {
      mTable = getBaseClass().getFunctionTable();
   }

   Functions::iterator it = mFunctions.begin();
   for ( ; it != mFunctions.end(); it++ )
   {
      ASTFunction* pfunction = it->second;
      mTable.insert(*pfunction);
   }

   mTable.reindex();
}

// - Search

const ASTField* ASTClass::findStatic(const std::string& name, SearchScope scope) const
{
   return const_cast<ASTClass&>(*this).findStatic(name, scope);
}

ASTField* ASTClass::findStatic(const std::string& name, SearchScope scope)
{
   for ( std::size_t index = 0; index < mStatics.size(); index++ )
   {
      ASTField* pstatic = mStatics[index];
      if ( pstatic->getVariable().getName() == name )
      {
         return pstatic;
      }
   }
   return scope == eAll && hasBaseClass() ? getBaseClass().findStatic(name, eAll) : NULL;
}

const ASTField* ASTClass::findField(const std::string& name, SearchScope scope) const
{
   return const_cast<ASTClass&>(*this).findField(name, scope);
}

ASTField* ASTClass::findField(const std::string& name, SearchScope scope)
{
   for ( std::size_t index = 0; index < mFields.size(); index++ )
   {
      ASTField* pvariable = mFields[index];
      if ( pvariable->getVariable().getName() == name )
      {
         return pvariable;
      }
   }
   return scope == eAll && hasBaseClass() ? getBaseClass().findField(name) : NULL;
}

const ASTFunction* ASTClass::findBestMatch(const std::string& name, const Signature& signature, const ASTTypeList& types) const
{
   return const_cast<ASTClass&>(*this).findBestMatch(name, signature, types);
}

ASTFunction* ASTClass::findBestMatch(const std::string& name, const Signature& signature, const ASTTypeList& types)
{
   Functions::iterator it = mFunctions.find(name);
   if ( it != mFunctions.end() )
   {
      Functions::iterator end = mFunctions.upper_bound(name);

      for ( ; it != end; it++ )
      {
         ASTFunction* pfunction = it->second;
         if ( pfunction->getSignature().bestMatch(signature, types) )
         {
            return pfunction;
         }
      }
   }

   return hasBaseClass() ? getBaseClass().findBestMatch(name, signature, types) : NULL;
}

const ASTFunction* ASTClass::findExactMatch(const std::string& name, const Signature& signature) const
{
   return const_cast<ASTClass&>(*this).findExactMatch(name, signature);
}

ASTFunction* ASTClass::findExactMatch(const std::string& name, const Signature& signature)
{
   ASTFunction* pfunction = findExactMatchLocal(name, signature);
   if ( pfunction == NULL && hasBaseClass() )
   {
      pfunction = getBaseClass().findExactMatch(name, signature);
   }
   return pfunction;
}

ASTFunction* ASTClass::findExactMatchLocal(const std::string& name, const Signature& signature)
{
   Functions::iterator it = mFunctions.find(name);
   if ( it != mFunctions.end() )
   {
      Functions::iterator end = mFunctions.upper_bound(name);

      for ( ; it != end; it++ )
      {
         ASTFunction* pfunction = it->second;
         if ( pfunction->getSignature().exactMatch(signature) )
         {
            return pfunction;
         }
      }
   }
   return NULL;
}

const ASTFunction* ASTClass::findInterfaceFunction(const ASTFunction& function) const
{
   for ( int index = 0; index < mInterfaces.size(); index++ )
   {
      const ASTType& type = mInterfaces[index];

      const ASTClass& c = type.getObjectClass();
      ASSERT(c.getKind() == ASTClass::eInterface);

      const ASTFunction* pifunc = c.findExactMatch(function.getName(), function.getSignature());
      if ( pifunc != NULL )
      {
         return pifunc;
      }
   }

   return NULL;
}

// - Visitor
   
ACCEPT_IMPL(ASTClass)
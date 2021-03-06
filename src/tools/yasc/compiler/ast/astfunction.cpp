
#include "astfunction.h"

#include "core/defines.h"
#include <exception>

#include "astblock.h"
#include "astfunctionargument.h"
#include "asttype.h"
#include "astvisitor.h"
#include "astvariable.h"
#include "astannotations.h"
#include "asttypevariables.h"
#include "astsignature.h"
#include "astlocalvariable.h"

ASTFunction::ASTFunction(ASTMember::Kind kind):
   ASTMember(kind),
   mName(),
   mArguments(),
   mLocals(),
   mpAnnotations(nullptr),
   mModifiers(),
   mpTypeVariables(nullptr),
   mpType(nullptr),
   mpClass(nullptr),
   mpBaseFunction(nullptr),
   mpBody(nullptr),
   mpSignature(new ASTSignature()),
   mResourceIndex(-1),
   mLocalVariableCount(0)
{
}

ASTFunction::~ASTFunction()
{
   setType(nullptr);
   setBody(nullptr);

   delete mpSignature;
}

// - Get/set

const String& ASTFunction::getName() const
{
   return mName;
}

void ASTFunction::setName(const String& name)
{
   mName = name;
}

const ASTType& ASTFunction::getType() const
{
   return *mpType;
}

ASTType& ASTFunction::getType()
{
   return *mpType;
}

void ASTFunction::setType(ASTType* ptype)
{
   delete mpType;
   mpType = ptype;
}

bool ASTFunction::hasTypeVariables() const
{
   return mpTypeVariables != nullptr;
}

const ASTTypeVariables& ASTFunction::getTypeVariables() const
{
   ASSERT_PTR(mpTypeVariables);
   return *mpTypeVariables;
}

ASTTypeVariables& ASTFunction::getTypeVariables()
{
   ASSERT_PTR(mpTypeVariables);
   return *mpTypeVariables;
}

void ASTFunction::setTypeVariables(ASTTypeVariables* ptypes)
{
   delete mpTypeVariables;
   mpTypeVariables = ptypes;
}

bool ASTFunction::hasAnnotations() const 
{
   return mpAnnotations != nullptr;
}

const ASTAnnotations& ASTFunction::getAnnotations() const
{
   return *mpAnnotations;
}

ASTAnnotations& ASTFunction::getAnnotations()
{
   return *mpAnnotations;
}

void ASTFunction::setAnnotations(ASTAnnotations* pannotations)
{
   delete mpAnnotations;
   mpAnnotations = pannotations;
}

const ASTModifiers& ASTFunction::getModifiers() const
{
   return mModifiers;
}

ASTModifiers& ASTFunction::getModifiers()
{
   return mModifiers;
}

void ASTFunction::setModifiers(const ASTModifiers& modifiers)
{
   mModifiers = modifiers;
}

int ASTFunction::getResourceIndex() const
{
   return mResourceIndex;
}

void ASTFunction::setResourceIndex(int index)
{
   mResourceIndex = index;
}

const ASTClass& ASTFunction::getClass() const
{
   ASSERT_PTR(mpClass);
   return *mpClass;
}
   
void ASTFunction::setClass(ASTClass& owner)
{
   mpClass = &owner;
}

const ASTFunction& ASTFunction::getBaseFunction() const
{
   ASSERT_PTR(mpBaseFunction);
   return *mpBaseFunction;
}
  
void ASTFunction::setBaseFunction(ASTFunction& function)
{
   mpBaseFunction = &function;
}

const ASTBlock& ASTFunction::getBody() const
{
   return *mpBody;
}

bool ASTFunction::hasBody() const
{
   return mpBody != nullptr;
}

ASTBlock& ASTFunction::getBody()
{
   return *mpBody;
}

void ASTFunction::setBody(ASTBlock* pbody)
{
   delete mpBody;
   mpBody = pbody;
}

const ASTSignature& ASTFunction::getSignature() const
{
   ASSERT(mpSignature);
   return *mpSignature;
}

const ASTTypeList& ASTFunction::getArguments() const
{
   return mArguments;
}

const ASTTypeList& ASTFunction::getLocals() const
{
   return mLocals;
}

int ASTFunction::getLocalVariableCount() const
{
   return mLocalVariableCount;
}

void ASTFunction::setLocalVariableCount(int count)
{
   mLocalVariableCount = count;
}

// - Query

bool ASTFunction::isConstructor() const
{
   return getKind() == eConstructor;
}

bool ASTFunction::isDefaultConstructor() const
{
   return isConstructor() && getChildren().isEmpty();
}

bool ASTFunction::isGeneric() const
{
   return mpTypeVariables != nullptr && mpTypeVariables->size() > 0;
}

bool ASTFunction::isVirtual() const
{
   return mpBaseFunction != nullptr;
}

const ASTNodes& ASTFunction::getArgumentNodes() const
{
   return getChildren();
}

int ASTFunction::getArgumentCount() const
{
   return getChildren().size() + (getModifiers().isStatic() ? 0 : 1);
}

String ASTFunction::getPrototype() const
{
   return getName() + '(' + mpSignature->toString() + ')';
}

bool ASTFunction::isTypeName(const String& name) const
{
   return isGeneric() && mpTypeVariables->contains(name);
}

// - Operations
   
void ASTFunction::addArgument(ASTFunctionArgument* pargument)
{
   mpSignature->append(pargument->getVariable().getType());

   pargument->getVariable().setResourceIndex(getArgumentCount());

   addChild(pargument);
}

void ASTFunction::addArgument(ASTType* pargument)
{
   mArguments.append(pargument);
}

void ASTFunction::addLocal(ASTType* plocal)
{
   mLocals.append(plocal);
}

void ASTFunction::cleanup()
{
   delete mpBody;
   mpBody = nullptr;
}

// - Visitor
   
ACCEPT_IMPL(ASTFunction)
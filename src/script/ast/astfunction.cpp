
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
   mpAnnotations(NULL),
   mModifiers(),
   mpTypeVariables(NULL),
   mpType(NULL),
   mpClass(NULL),
   mpBaseFunction(NULL),
   mpBody(NULL),
   mpSignature(new ASTSignature()),
   mInstructions(),
   mGuards(),
   mSwitchTables(),
   mResourceIndex(-1),
   mLocalVariableCount(0)
{
}

ASTFunction::~ASTFunction()
{
   setType(NULL);
   setBody(NULL);

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
   return mpTypeVariables != NULL;
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
   return mpAnnotations != NULL;
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
   return mpBody != NULL;
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

const CIL::Instructions& ASTFunction::getInstructions() const
{
   return mInstructions;
}

void ASTFunction::setInstructions(const CIL::Instructions& instructions)
{
   mInstructions = instructions;
}

const CIL::Guards& ASTFunction::getGuards() const
{
   return mGuards;
}
 
void ASTFunction::setGuards(const CIL::Guards& guards)
{
   mGuards = guards;
}

const CIL::SwitchTables& ASTFunction::getSwitchTables() const
{
   return mSwitchTables;
}

void ASTFunction::setSwitchTables(const CIL::SwitchTables& tables)
{
   mSwitchTables = tables;
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
   return mpTypeVariables != NULL && mpTypeVariables->size() > 0;
}

bool ASTFunction::isVirtual() const
{
   return mpBaseFunction != NULL;
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
   mpBody = NULL;
}

// - Visitor
   
ACCEPT_IMPL(ASTFunction)
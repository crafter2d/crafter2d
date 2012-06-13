
#include "astfunction.h"

#include "core/defines.h"
#include <exception>

#include "script/compiler/signature.h"

#include "astblock.h"
#include "astfunctionargument.h"
#include "asttype.h"
#include "astvisitor.h"
#include "astvariable.h"
#include "astannotations.h"
#include "asttypevariables.h"

ASTFunction::ASTFunction(ASTMember::Kind kind):
   ASTMember(kind),
   mName(),
   mpAnnotations(NULL),
   mModifiers(),
   mpTypeVariables(NULL),
   mpType(NULL),
   mpClass(NULL),
   mpBody(NULL),
   mpSignature(new Signature()),
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

const std::string& ASTFunction::getName() const
{
   return mName;
}

void ASTFunction::setName(const std::string& name)
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

const Signature& ASTFunction::getSignature() const
{
   ASSERT(mpSignature);
   return *mpSignature;
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

bool ASTFunction::isGeneric() const
{
   return mpTypeVariables != NULL && mpTypeVariables->size() > 0;
}

int ASTFunction::getArgumentCount() const
{
   return getChildren().size() + (getModifiers().isStatic() ? 0 : 1);
}

// - Operations
   
void ASTFunction::addArgument(ASTFunctionArgument* pargument)
{
   mpSignature->append(pargument->getVariable().getType());

   pargument->getVariable().setResourceIndex(getArgumentCount());

   addChild(pargument);
}

const ASTNodes& ASTFunction::getArguments() const
{
   return getChildren();
}

// - Search
   
const ASTFunctionArgument& ASTFunction::resolveArgument(const std::string& name) const
{
   for ( int index = 0; index < getChildren().size(); index++ )
   {
      const ASTFunctionArgument& argument = static_cast<const ASTFunctionArgument&>(getChildren()[index]);
      if ( argument.getVariable().getName() == name )
      {
         return argument;
      }
   }

   UNREACHABLE("unreachable!");
}

// - Visitor
   
ACCEPT_IMPL(ASTFunction)

#include "astaccess.h"

#include "core/defines.h"

#include "astfield.h"
#include "astvisitor.h"
#include "astexpression.h"
#include "asttype.h"

ASTAccess::ASTAccess():
   ASTExpressionPart(),
   mName(),
   mKind(eInvalid),
   mVarAccess(eInvalidAccess),
   mpNext(NULL),
   mpPrevious(NULL),
   mpFunction(NULL),
   mpField(NULL),
   mpVariable(NULL),
   mpStaticType(NULL),
   mTypeArguments()
{
}

ASTAccess::~ASTAccess()
{
   setStaticType(NULL);
}

// - Get/set
   
const String& ASTAccess::getName() const
{
   return mName;
}

void ASTAccess::setName(const String& name)
{
   mName = name;
}

ASTAccess::Kind ASTAccess::getKind() const
{
   return mKind;
}

void ASTAccess::setKind(Kind kind)
{
   mKind = kind;
}

ASTAccess::VarAccess ASTAccess::getAccess() const
{
   return mVarAccess;
}

void ASTAccess::setAccess(VarAccess access)
{
   mVarAccess = access;
}

bool ASTAccess::hasNext() const
{
   return mpNext != NULL;
}

ASTAccess& ASTAccess::getNext()
{
   return *mpNext;
}

void ASTAccess::setNext(ASTAccess& next)
{
   mpNext = &next;
}

bool ASTAccess::hasPrevious() const
{
   return mpPrevious != NULL;
}

ASTAccess& ASTAccess::getPrevious()
{
   ASSERT_PTR(mpPrevious);
   return *mpPrevious;
}

void ASTAccess::setPrevious(ASTAccess& previous)
{
   mpPrevious = &previous;
}

const ASTNodes& ASTAccess::getArguments() const
{
   return getChildren();
}

ASTNodes& ASTAccess::getArguments()
{
   return getChildren();
}

const ASTFunction& ASTAccess::getFunction() const
{
   return *mpFunction;
}

void ASTAccess::setFunction(const ASTFunction& function)
{
   mpFunction = &function;
}

const ASTField& ASTAccess::getField() const
{
   ASSERT_PTR(mpField);
   ASSERT(mVarAccess == eField || mVarAccess == eRefField);
   return *mpField;
}

void ASTAccess::setField(const ASTField& field)
{
   ASSERT(mVarAccess == eField || mVarAccess == eRefField);
   mpField = &field;
}

const ASTVariable& ASTAccess::getVariable() const
{
   ASSERT(mKind == eVariable);
   if ( mVarAccess == eField || mVarAccess == eRefField )
      return mpField->getVariable();
   return  *mpVariable;
}

void ASTAccess::setVariable(const ASTVariable& variable)
{
   mpVariable = &variable;
}

const ASTType& ASTAccess::getStaticType() const
{
   return *mpStaticType;
}

void ASTAccess::setStaticType(ASTType* ptype)
{
   delete mpStaticType;
   mpStaticType = ptype;
}

bool ASTAccess::hasTypeArguments() const
{
   return !mTypeArguments.empty();
}

const ASTTypeList& ASTAccess::getTypeArguments() const
{
   return mTypeArguments;
}

ASTTypeList& ASTAccess::getTypeArguments()
{
   return mTypeArguments;
}

// operations
   
void ASTAccess::addArgument(ASTExpression* pexpression)
{
   addChild(pexpression);
}

void ASTAccess::replaceArgument(int index, ASTNode* pnode)
{
   getChildren().set(index, pnode);
}

// - Visitor

ACCEPT_IMPL(ASTAccess)

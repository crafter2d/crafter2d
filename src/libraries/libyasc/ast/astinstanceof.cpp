
#include "astinstanceof.h"

#include "core/defines.h"

#include "asttype.h"
#include "astvisitor.h"

ASTInstanceOf::ASTInstanceOf():
   ASTNode(),
   mpInstanceType(nullptr),
   mpObject(nullptr)
{
}

ASTInstanceOf::~ASTInstanceOf()
{
   setInstanceType(nullptr);
   setObject(nullptr);
}

// - Get/set

const ASTType& ASTInstanceOf::getInstanceType() const
{
   ASSERT_PTR(mpInstanceType);
   return *mpInstanceType;
}

ASTType& ASTInstanceOf::getInstanceType()
{
   ASSERT_PTR(mpInstanceType);
   return *mpInstanceType;
}

void ASTInstanceOf::setInstanceType(ASTType* ptype)
{
   delete mpInstanceType;
   mpInstanceType = ptype;
}

const ASTNode& ASTInstanceOf::getObject() const
{
   ASSERT_PTR(mpObject);
   return *mpObject;
}

ASTNode& ASTInstanceOf::getObject()
{
   ASSERT_PTR(mpObject);
   return *mpObject;
}

void ASTInstanceOf::setObject(ASTNode* pobject)
{
   delete mpObject;
   mpObject = pobject;
}

// - Visiting
   
ACCEPT_IMPL(ASTInstanceOf)

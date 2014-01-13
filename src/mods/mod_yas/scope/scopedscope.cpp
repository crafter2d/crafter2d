
#include "scopedscope.h"

#include "scope.h"
#include "scopestack.h"

ScopedScope::ScopedScope(ScopeStack& stack):
   mStack(stack)
{
   mStack.push(new Scope());
}

ScopedScope::~ScopedScope()
{
   Scope* pscope = mStack.pop();
   delete pscope;
}

// - Operators
   
Scope& ScopedScope::operator*()
{
   return mStack.top();
}

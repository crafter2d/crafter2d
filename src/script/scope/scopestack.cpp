
#include "scopestack.h"

#include "scope.h"

ScopeStack::ScopeStack():
   mScopes()
{
}

// - Query

Scope& ScopeStack::top()
{
   return *mScopes.back();
}

// - Operations

void ScopeStack::add(ScopeVariable* pvariable)
{
   top().add(pvariable);
}
   
void ScopeStack::push(Scope* pscope)
{
   mScopes.push_back(pscope);
}
   
Scope* ScopeStack::pop()
{
   Scope* pscope = mScopes.back();
   mScopes.pop_back();
   return pscope;
}

// search
   
ScopeVariable* ScopeStack::find(const std::string& name)
{
   for ( int index = mScopes.size() - 1; index >= 0; index-- )
   {
      Scope* pscope = mScopes[index];
      ScopeVariable* pvariable = pscope->find(name);
      if ( pvariable != NULL )
      {
         return pvariable;
      }
   }

   return NULL;
}

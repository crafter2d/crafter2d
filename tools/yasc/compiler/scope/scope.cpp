
#include "scope.h"

#include "core/string/string.h"

#include "scopevariable.h"

Scope::Scope():
   mVariables()
{
}

Scope::~Scope()
{
   clear();
}

// - Operations

void Scope::add(ScopeVariable* pvariable)
{
   mVariables.push_back(pvariable);
}

void Scope::clear()
{
   for ( std::size_t index = 0; index < mVariables.size(); index++ )
   {
      ScopeVariable* pvariable = mVariables[index];
      delete pvariable;
   }
}

// - Search

ScopeVariable* Scope::find(const String& name)
{
   for ( std::size_t index = 0; index < mVariables.size(); index++ )
   {
      ScopeVariable* pvariable = mVariables[index];
      if ( pvariable->getName() == name )
      {
         return pvariable;
      }
   }
   return NULL;
}

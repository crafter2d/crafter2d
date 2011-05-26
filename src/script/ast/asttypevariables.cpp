
#include "asttypevariables.h"

#include "core/defines.h"

#include "asttypevariable.h"

ASTTypeVariables::ASTTypeVariables():
   mTypeVariables()
{
}

// - Query

bool ASTTypeVariables::contains(const ASTTypeVariable& variable) const
{
   return mTypeVariables.find(variable.getIdentifier()) != mTypeVariables.end();
}

int ASTTypeVariables::size() const
{
   return mTypeVariables.size();
}

// - Operations

void ASTTypeVariables::append(ASTTypeVariable* pvariable)
{
   ASSERT(!contains(*pvariable));
   pvariable->setIndex(mTypeVariables.size());
   mTypeVariables[pvariable->getIdentifier()] = pvariable;
}

void ASTTypeVariables::clear()
{
   TypeVariableMap::iterator it = mTypeVariables.begin();
   for ( ; it != mTypeVariables.end(); it++ )
   {
      delete it->second;
   }
   mTypeVariables.clear();
}

// - Search
   
const ASTTypeVariable* ASTTypeVariables::find(const std::string& id) const
{
   TypeVariableMap::const_iterator it = mTypeVariables.find(id);
   return it != mTypeVariables.end() ? it->second : NULL;
}


#include "astfunctionmap.h"

#include "astfunction.h"

ASTFunctionMap::ASTFunctionMap():
   mFunctions()
{
}

void ASTFunctionMap::insert(ASTFunction* pfunction)
{
   mFunctions.insert(std::make_pair(pfunction->getName(), pfunction));
}

// - Lookup

ASTFunction* ASTFunctionMap::findBestMatch(const ASTFunction& that, const ASTTypeList& types)
{
   return findBestMatch(that.getName(), that.getSignature(), types);
}
   
ASTFunction* ASTFunctionMap::findExactMatch(const ASTFunction& that)
{
   return findExactMatch(that.getName(), that.getSignature());
}

ASTFunction* ASTFunctionMap::findBestMatch(const String& name, const Signature& signature, const ASTTypeList& types)
{
   FunctionMap::iterator it = mFunctions.find(name);
   if ( it != mFunctions.end() )
   {
      FunctionMap::iterator end = mFunctions.upper_bound(name);

      for ( ; it != end; ++it )
      {
         ASTFunction* pfunction = it->second;
         if ( pfunction->getSignature().bestMatch(signature, types) )
         {
            return pfunction;
         }
      }
   }
}

ASTFunction* ASTFunctionMap::findExactMatch(const String& name, const Signature& signature)
{
   FunctionMap::iterator it = mFunctions.find(name);
   if ( it != mFunctions.end() )
   {
      FunctionMap::iterator end = mFunctions.upper_bound(name);

      for ( ; it != end; ++it )
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
}

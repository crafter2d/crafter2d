
#include "astfunctionmap.h"

#include "astfunction.h"
#include "astsignature.h"
#include "astvisitor.h"

ASTFunctionMap::ASTFunctionMap():
   mFunctions()
{
}

ASTFunctionMap::~ASTFunctionMap()
{
   clear();
}

// - Query

bool ASTFunctionMap::hasConstructor() const
{
   FunctionMap::const_iterator it = mFunctions.begin();
   for ( ; it != mFunctions.end(); ++it )
   {
      ASTFunction* pfunction = it->second;
      if ( pfunction->getKind() == ASTMember::eConstructor )
      {
         return true;
      }
   }
   return false;
}

bool ASTFunctionMap::hasAbstractFunction() const
{
   FunctionMap::const_iterator it = mFunctions.begin();
   for ( ; it != mFunctions.end(); ++it )
   {
      ASTFunction* pfunction = it->second;
      if ( pfunction->getModifiers().isAbstract() )
      {
         return true;
      }
   }
   return false;
}

bool ASTFunctionMap::hasNativeFunction() const
{
   FunctionMap::const_iterator it = mFunctions.begin();
   for ( ; it != mFunctions.end(); ++it )
   {
      ASTFunction* pfunction = it->second;
      if ( pfunction->getModifiers().isNative() )
      {
         return true;
      }
   }
   return false;
}

bool ASTFunctionMap::hasNativeConstructor() const
{
   FunctionMap::const_iterator it = mFunctions.begin();
   for ( ; it != mFunctions.end(); ++it )
   {
      ASTFunction* pfunction = it->second;
      if ( pfunction->getKind() == ASTMember::eConstructor
         && ( pfunction->getModifiers().isNative()
           || pfunction->getModifiers().isPureNative() ) )
      {
         return true;
      }
   }
   return false;
}

bool ASTFunctionMap::hasFunction(const String& name) const
{
   return mFunctions.find(name) != mFunctions.end();
}

// - Iterator

ASTFunctionMap::Iterator ASTFunctionMap::getIterator()
{
   return mFunctions.begin();
}
   
bool ASTFunctionMap::hasNext(Iterator& it) const
{
   return it != mFunctions.end();
}

ASTFunction& ASTFunctionMap::getNext(Iterator& it)
{
   return *(it++)->second;
}

// - Operations

void ASTFunctionMap::insert(ASTFunction* pfunction)
{
   mFunctions.insert(std::make_pair(pfunction->getName(), pfunction));
}

void ASTFunctionMap::clear()
{
   FunctionMap::iterator it = mFunctions.begin();
   for ( ; it != mFunctions.end(); ++it )
   {
      delete it->second;
   }
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

ASTFunction* ASTFunctionMap::findBestMatch(const String& name, const ASTSignature& signature, const ASTTypeList& types)
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
   return nullptr;
}

ASTFunction* ASTFunctionMap::findExactMatch(const String& name, const ASTSignature& signature)
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
   return nullptr;
}

// - Visitor

void ASTFunctionMap::accept(ASTVisitor& visitor) const
{
   FunctionMap::const_iterator it = mFunctions.begin();
   for ( ; it != mFunctions.end(); ++it )
   {
      const ASTFunction* pfunction = it->second;
      visitor.visit(*pfunction);
   }
}
   
void ASTFunctionMap::accept(ASTVisitor& visitor)
{
   FunctionMap::iterator it = mFunctions.begin();
   for ( ; it != mFunctions.end(); ++it )
   {
      ASTFunction* pfunction = it->second;
      visitor.visit(*pfunction);
   }
}
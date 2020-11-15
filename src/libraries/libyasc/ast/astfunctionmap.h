
#ifndef AST_FUNCTION_MAP_H
#define AST_FUNCTION_MAP_H

#include <map>
#include "core/string/string.h"

class ASTFunction;
class ASTTypeList;
class ASTSignature;
class ASTVisitor;

class ASTFunctionMap
{
   typedef std::multimap<String, ASTFunction*> FunctionMap;
public:

   typedef FunctionMap::iterator Iterator;

   ASTFunctionMap();
   ~ASTFunctionMap();

 // query
   bool hasConstructor() const;
   bool hasAbstractFunction() const;
   bool hasNativeFunction() const;
   bool hasNativeConstructor() const;

   bool hasFunction(const String& name) const;

 // iterators
   Iterator     getIterator();
   bool         hasNext(Iterator& it) const;
   ASTFunction& getNext(Iterator& it);

 // maintenance
   void insert(ASTFunction* pfunction);
   void clear();

 // lookup
   ASTFunction* findBestMatch(const ASTFunction& that, const ASTTypeList& types);
   ASTFunction* findExactMatch(const ASTFunction& that);

   ASTFunction* findBestMatch(const String& name, const ASTSignature& signature, const ASTTypeList& types);
   ASTFunction* findExactMatch(const String& name, const ASTSignature& signature);

 // visitor
   void accept(ASTVisitor& visitor) const;
   void accept(ASTVisitor& visitor);

private:
   FunctionMap mFunctions;
};

#endif // AST_FUNCTION_MAP_H

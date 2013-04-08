
#ifndef AST_FUNCTION_MAP_H
#define AST_FUNCTION_MAP_H

#include <map>
#include "core/string/string.h"

class ASTFunction;
class ASTTypeList;
class Signature;

class ASTFunctionMap
{
   typedef std::multimap<String, ASTFunction*> FunctionMap;
public:
   ASTFunctionMap();

 // maintenance
   void insert(ASTFunction* pfunction);

 // lookup
   ASTFunction* findBestMatch(const ASTFunction& that, const ASTTypeList& types);
   ASTFunction* findExactMatch(const ASTFunction& that);

   ASTFunction* findBestMatch(const String& name, const Signature& signature, const ASTTypeList& types);
   ASTFunction* findExactMatch(const String& name, const Signature& signature);

private:
   FunctionMap mFunctions;
};

#endif // AST_FUNCTION_MAP_H


#ifndef AST_TYPE_VARIABLES_H_
#define AST_TYPE_VARIABLES_H_

#include <map> 

#include "core/string/string.h"

class ASTTypeVariable;

class ASTTypeVariables
{
   typedef std::map<String, ASTTypeVariable*> TypeVariableMap;

public:
   ASTTypeVariables();

 // query
   bool contains(const String& name) const;
   bool contains(const ASTTypeVariable& variable) const;
   int size() const;

 // operations
   void append(ASTTypeVariable* pvariable);
   void clear();

 // search
   const ASTTypeVariable* find(const String& id) const;

private:

   TypeVariableMap mTypeVariables;
};

#endif // AST_TYPE_VARIABLES_H_

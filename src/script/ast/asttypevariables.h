
#ifndef AST_TYPE_VARIABLES_H_
#define AST_TYPE_VARIABLES_H_

#include <map> 
#include <string>

class ASTTypeVariable;

class ASTTypeVariables
{
   typedef std::map<std::string, ASTTypeVariable*> TypeVariableMap;

public:
   ASTTypeVariables();

 // query
   bool contains(const std::string& name) const;
   bool contains(const ASTTypeVariable& variable) const;
   int size() const;

 // operations
   void append(ASTTypeVariable* pvariable);
   void clear();

 // search
   const ASTTypeVariable* find(const std::string& id) const;

private:
   friend class ASTStream;

   TypeVariableMap mTypeVariables;
};

#endif // AST_TYPE_VARIABLES_H_

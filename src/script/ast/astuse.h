
#ifndef AST_USE_H_
#define AST_USE_H_

#include <string>

#include "astnode.h"

class ASTUse : public ASTNode
{
public:
   ASTUse();

 // get/set
   const std::string& getIdentifier() const;
   void           setIdentifier(const std::string& identifier);

 // visitor
   ACCEPT;

private:
   std::string mIdentifier;
};

#endif // AST_USE_H_

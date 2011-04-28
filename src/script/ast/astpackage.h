
#ifndef AST_PACKAGE_H_
#define AST_PACKAGE_H_

#include <string>

#include "astnode.h"

class ASTPackage : public ASTNode
{
public:
   ASTPackage();

 // get/set
   const std::string& getName() const;
   void setName(const std::string& name);

 // visit
   ACCEPT;

private:
   std::string mName;
};

#endif // AST_PACKAGE_H_

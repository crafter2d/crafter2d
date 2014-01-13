
#ifndef AST_PACKAGE_H_
#define AST_PACKAGE_H_

#include "core/string/string.h"

#include "astnode.h"

class ASTPackage : public ASTNode
{
public:
   ASTPackage();

 // get/set
   const String& getName() const;
   void          setName(const String& name);

 // visit
   ACCEPT;

private:
   String mName;
};

#endif // AST_PACKAGE_H_

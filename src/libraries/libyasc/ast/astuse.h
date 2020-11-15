
#ifndef AST_USE_H_
#define AST_USE_H_

#include "core/string/string.h"

#include "astnode.h"

class ASTUse : public ASTNode
{
public:
   ASTUse();

 // get/set
   const String& getIdentifier() const;
   void          setIdentifier(const String& identifier);

 // visitor
   ACCEPT;

private:
   String mIdentifier;
};

#endif // AST_USE_H_

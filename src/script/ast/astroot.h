
#ifndef AST_ROOT_H_
#define AST_ROOT_H_

#include <string>
#include <vector>

#include "astnode.h"

class ASTRoot : public ASTNode
{
public:
   typedef std::vector<std::string> StringList;

   ASTRoot();

 // get/set
   const StringList& getUses() const;

 // operators
   void addUse(const std::string& use);

 // visitor
   ACCEPT;

private:
   StringList mUses;
};

#endif // AST_ROOT_H_

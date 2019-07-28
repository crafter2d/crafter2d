
#ifndef AST_PROPERTY_H
#define AST_PROPERTY_H

#include "core/string/string.h"

#include "astmember.h"

class ASTProperty : public ASTMember
{
public:
   ASTProperty();
   virtual ~ASTProperty();

 // get/set
   const String& getName() const;
   void          setName(const String& name);

   const ASTType& getType() const;
         ASTType& getType();
   void           setType(ASTType* ptype);

 // visitor
   ACCEPT;

private:

 // data
   String   mName;
   ASTType* mpType; // owns
};

#endif // AST_PROPERTY_H

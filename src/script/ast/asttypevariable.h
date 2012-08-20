
#ifndef AST_TYPE_VARIABLE_H_
#define AST_TYPE_VARIABLE_H_

#include "core/string/string.h"

class ASTType;

class ASTTypeVariable
{
public:
   enum Mode { eNormal, eExtends, eSuper };

   ASTTypeVariable();
   ~ASTTypeVariable();

 // get/set
   const String& getIdentifier() const;
   void          setIdentifier(const String& id);

   Mode getMode() const;
   void setMode(Mode mode);

   const ASTType& getOther() const;
   void           setOther(ASTType* ptype);

   int  getIndex() const;
   void setIndex(int index);

private:
   String mId;
   Mode        mMode;
   ASTType*    mpOther;
   int         mIndex;
};

#endif // AST_TYPE_VARIABLE_H_

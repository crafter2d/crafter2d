
#ifndef AST_TYPE_VARIABLE_H_
#define AST_TYPE_VARIABLE_H_

#include <string>

class ASTType;

class ASTTypeVariable
{
public:
   enum Mode { eNormal, eExtends, eSuper };

   ASTTypeVariable();
   ~ASTTypeVariable();

 // get/set
   const std::string& getIdentifier() const;
   void               setIdentifier(const std::string& id);

   Mode getMode() const;
   void setMode(Mode mode);

   const ASTType& getOther() const;
   void           setOther(ASTType* ptype);

   int  getIndex() const;
   void setIndex(int index);

private:
   std::string mId;
   Mode        mMode;
   ASTType*    mpOther;
   int         mIndex;
};

#endif // AST_TYPE_VARIABLE_H_

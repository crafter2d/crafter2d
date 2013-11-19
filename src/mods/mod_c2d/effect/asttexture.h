
#ifndef AST_TEXTURE_H
#define AST_TEXTURE_H

#include "core/string/string.h"

class ASTTexture
{
public:
   ASTTexture(): mName(), mRegister(-1) {}

   String mName;
   int    mRegister;

};

#endif // AST_TEXTURE_H

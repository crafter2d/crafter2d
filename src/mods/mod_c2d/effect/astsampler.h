
#ifndef AST_SAMPLER_H
#define AST_SAMPLER_H

#include "core/string/string.h"

class ASTSampler
{
public:
   ASTSampler() : mName(), mRegister(-1) {}

   String mName;
   int    mRegister;
};

#endif // AST_SAMPLER_H

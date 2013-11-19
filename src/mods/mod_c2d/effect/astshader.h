
#ifndef AST_SHADER_H
#define AST_SHADER_H

#include "core/streams/bufferedstream.h"
#include "core/string/string.h"

class ASTShader
{
public:
   String mTarget;
   String mEntry;
   BufferedStream mCompiledCode;
};

#endif // AST_SHADER_H

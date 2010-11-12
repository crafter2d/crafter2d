

#ifndef CODE_BLOCKED_STREAM_H_
#define CODE_BLOCKED_STREAM_H_

#include "codestream.h"


class CodeBlock
{
public:
   static CodeStream& endl(CodeStream& stream)
   {
      stream << CodeStream::endl;
      for ( int i = 1; i < sDepth; i++ )
      {
         stream << "   ";
      }
      return stream;
   }

   CodeBlock()
   {
      sDepth++;
   }

   ~CodeBlock()
   {
      sDepth--;
   }

private:
   static int sDepth;
};

#endif // CODE_BLOCKED_STREAM_H_

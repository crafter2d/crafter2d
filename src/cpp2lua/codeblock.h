

#ifndef CODE_BLOCKED_STREAM_H_
#define CODE_BLOCKED_STREAM_H_

#include "codestream.h"

class CodeBlockedStream : public CodeStream
{
public:
   static CodeStream& endline(CodeStream& stream)
   {
      sNewLine = true;
      stream << CodeStream::endl;
      return stream;
   }

   CodeBlockedStream(CodeStream& stream):
      mStream(stream)
   {
      sDepth++;
   }

   ~CodeBlockedStream()
   {
      sDepth--;
   }

   virtual CodeStream& operator<<(const std::string& text);
   virtual CodeStream& operator<<(int value);

private:
   void writeIndentation();

   static int  sDepth;
   static bool sNewLine;

   CodeStream& mStream;
};

#endif // CODE_BLOCKED_STREAM_H_

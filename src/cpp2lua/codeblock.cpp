

#include "codeblock.h"

int CodeBlockedStream::sDepth = 0;
bool CodeBlockedStream::sNewLine = false;

CodeStream& CodeBlockedStream::operator<<(const std::string& text)
{
   if ( sNewLine )
      writeIndentation();
   mStream << text;
   return *this;
}

CodeStream& CodeBlockedStream::operator<<(int value)
{
   if ( sNewLine )
      writeIndentation();
   mStream << value;
   return *this;
}

void CodeBlockedStream::writeIndentation()
{
   for ( int index = 0; index < sDepth; index++ )
   {
      mStream << "   ";
   }
   sNewLine = false;
}

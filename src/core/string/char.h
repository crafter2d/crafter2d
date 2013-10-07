
#ifndef CHAR_H
#define CHAR_H

#include "core/core_base.h"

typedef wchar_t UChar;

class CORE_API Char
{
public:
   static bool isEOL(UChar c);
   static bool isWhitespace(UChar c);
   static bool isAlphaNum(UChar c);
   static bool isAlpha(UChar c);
   static bool isDigit(UChar c);
};

#endif // CHAR_H

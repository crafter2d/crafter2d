
#include "char.h"

#include <cwctype>

// static

bool Char::isEOL(UChar c)
{
   return c == '\r' || c == '\n';
}

bool Char::isWhitespace(UChar c)
{
   return iswspace(c);
}

bool Char::isAlphaNum(UChar c)
{
   return iswalnum(c);
}

bool Char::isAlpha(UChar c)
{
   return iswalpha(c);
}

bool Char::isDigit(UChar c)
{
   return iswdigit(c);
}
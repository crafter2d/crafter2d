
#include "char.h"

#include <unicode/utypes.h>
#include <unicode/uchar.h>

// static

bool Char::isEOL(UChar c)
{
   return c == '\r' || c == '\n';
}

bool Char::isWhitespace(UChar c)
{
   return u_isWhitespace(c) == TRUE;
}

bool Char::isAlphaNum(UChar c)
{
   return u_isalnum(c) == TRUE;
}

bool Char::isAlpha(UChar c)
{
   return u_isalpha(c) == TRUE;
}

bool Char::isDigit(UChar c)
{
   return u_isdigit(c) == TRUE;
}
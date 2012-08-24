
#include "char.h"

#include <unicode/utypes.h>
#include <unicode/uchar.h>

// static

bool Char::isEOL(char c)
{
   return c == '\r' || c == '\n';
}

bool Char::isWhitespace(char c)
{
   return u_isWhitespace(c) == TRUE;
}

bool Char::isAlphaNum(char c)
{
   return u_isalnum(c) == TRUE;
}

bool Char::isAlpha(char c)
{
   return u_isalpha(c) == TRUE;
}

bool Char::isDigit(char c)
{
   return u_isdigit(c) == TRUE;
}

#ifndef CHAR_H
#define CHAR_H

#include "core/core_base.h"

class CORE_API Char
{
public:
   static bool isWhitespace(char c);
   static bool isAlphaNum(char c);
   static bool isAlpha(char c);
   static bool isDigit(char c);
};

#endif // CHAR_H


#ifndef LEXER_H
#define LEXER_H

#include "core/string/string.h"

class Lexer
{
public:
   Lexer(const String& data);

 // get/set
   int getLine() const {
      return mLine;
   }
   int getColumn() const {
      return mColumn;
   }

 // query
   UChar getChar();
   bool next(UChar ch);
   bool next(const String& str);

   bool isEOF() const;

 // lexing
   String getToken();
   String getIdentifier();
   int    getNumber();

private:

 // helpers
   void skipWhitespace();
   void skipComment();

 // data
   const String&  mData;
   int            mPos;
   int            mLine;
   int            mColumn;
};

#endif // LEXER_H

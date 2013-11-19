
#include "lexer.h"

#include "core/conv/numberconverter.h"
#include "core/string/char.h"

Lexer::Lexer(const String& data):
   mData(data),
   mPos(0),
   mLine(0),
   mColumn(0)
{
}

// - Query

UChar Lexer::getChar()
{
   mColumn++;
   return mData[mPos++];
}

bool Lexer::next(UChar ch)
{
   skipWhitespace();

   if ( mData[mPos] == ch )
   {
      mPos++;
      mColumn++;
      return true;
   }

   return false;
}

bool Lexer::isEOF() const
{
   int length = mData.length();
   return mPos == mData.length();
}

// - Tokenizing

String Lexer::getToken()
{
   skipWhitespace();

   String result;
   while ( !isEOF() )
   {
      UChar ch = mData[mPos++];
      if ( Char::isWhitespace(ch) )
      {
         break;
      }
      result += ch;
   }

   return result;
}

int Lexer::getNumber()
{
   skipWhitespace();

   String number;
   while ( !isEOF())
   {
      UChar ch = mData[mPos++];
      if ( Char::isWhitespace(ch) || !Char::isDigit(ch) )
      {
         break;
      }

      number += ch;
   }

   return NumberConverter::getInstance().toInt(number);
}

String Lexer::getIdentifier()
{
   skipWhitespace();

   String result;
   if ( !isEOF() )
   {
      UChar ch = mData[mPos++];
      if ( Char::isAlpha(ch) || ch == L'_' )
      {
         result += ch;
         while ( !isEOF() )
         {
            ch = mData[mPos];
            if ( Char::isAlphaNum(ch) || ch == L'_' )
            {
               result += ch;
               mPos++;
            }
            else 
            {
               break;
            }         
         }
      }
   }
   
   return result;
}

// - Helpers

void Lexer::skipWhitespace()
{
   while ( !isEOF() )
   {
      UChar ch = mData[mPos];
      if ( ch == '/' )
      {
         skipComment();
         continue;
      }
      else if ( !Char::isWhitespace(ch) )
      {
         break;
      }

      if ( ch == '\n' || '\r' )
      {
         mLine++;
         mColumn = 0;
      }

      mColumn++;
      mPos++;
   }
}

void Lexer::skipComment()
{
   while ( true )
   {
      UChar ch = mData[mPos++];
      if ( ch == '\n' )
      {
         break;
      }
   }
}

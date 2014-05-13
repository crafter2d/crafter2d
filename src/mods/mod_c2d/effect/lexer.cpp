
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

   if ( !isEOF() && mData[mPos] == ch )
   {
      mPos++;
      mColumn++;
      skipWhitespace();
      return true;
   }

   return false;
}

bool Lexer::next(const String& token)
{
   skipWhitespace();

   bool result = false;
   if ( token.length() < (mData.length() - mPos) )
   {
      int pos = mPos;
      for ( uint32_t index = 0; index < token.length(); ++index, ++pos )
      {
         if ( token[index] != mData[pos] )
         {
            return false;
         }
      }

      mPos = pos;
      result = true;
   }

   skipWhitespace();

   return result;
}

bool Lexer::isEOF() const
{
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

   skipWhitespace();

   return result;
}

int Lexer::getNumber()
{
   skipWhitespace();

   String number;
   while ( !isEOF())
   {
      UChar ch = mData[mPos];
      if ( Char::isWhitespace(ch) || !Char::isDigit(ch) )
      {
         break;
      }

      ++mPos;
      number += ch;
   }

   skipWhitespace();

   return NumberConverter::getInstance().toInt(number);
}

String Lexer::getIdentifier()
{
   skipWhitespace();

   String result;
   if ( !isEOF() )
   {
      UChar ch = mData[mPos];
      if ( Char::isAlpha(ch) || ch == L'_' )
      {
         mPos++;
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

   skipWhitespace();
   
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

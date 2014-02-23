/***************************************************************************
 *   Copyright (C) 2012 by Jeroen Broekhuizen                              *
 *   jengine.sse@live.nl                                                   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU Library General Public License as       *
 *   published by the Free Software Foundation; either version 2 of the    *
 *   License, or (at your option) any later version.                       *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU Library General Public     *
 *   License along with this program; if not, write to the                 *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#include "string.h"

#include <string.h>
#include <functional>

#include "core/defines.h"

#ifdef WIN32
#  include "winstring.h"
#endif

#include "char.h"

int hex_digit(char c)
{
    return ((c >= '0' && c <= '9') ||
            (c >= 'A' && c <= 'F') ||
            (c >= 'a' && c <= 'f'));
}

struct Escape
{
   wchar_t source;
   wchar_t escape;
} 
Escapes[] = 
{ 
   { 't', '\t' },
   { 'r', '\r' },
   { 'n', '\n' },
   { 'a', '\a' },
   { '\\', '\\' },
   { '\'', '\'' },
   { '\"', '\"' },
};

int EscapeCount = sizeof(Escapes) / sizeof(Escape);

wchar_t convertEscape(wchar_t escape)
{
   for ( int index = 0; index < EscapeCount; ++index )
   {
      if ( Escapes[index].source == escape )
      {
         return Escapes[index].escape;
      }
   }
   return escape;
}

const String String::sEmpty;

const String& String::empty()
{
   return sEmpty;
}

String String::fromUtf8(const char* pdata)
{
   String result;
   result.setToUtf8(pdata, strlen(pdata));
   return result;
}

String::String():
   mpString(NULL),
   mCapacity(0),
   mLength(0)
{
}

String::String(const UChar* pdata):
   mpString(NULL),
   mCapacity(0),
   mLength(0)
{
   setTo(pdata, wcslen(pdata));
}

String::String(const std::string& that):
   mpString(NULL),
   mCapacity(0),
   mLength(0)
{
   setToUtf8(that.c_str(), that.length());
}

String::String(const std::wstring& that):
   mpString(NULL),
   mCapacity(0),
   mLength(0)
{
   setTo(that.c_str(), that.length());
}

String::String(const String& that):
   mpString(NULL),
   mCapacity(0),
   mLength(0)
{
   wchar_t* pdata = getBuffer(that.mLength);
   wmemcpy(pdata, that.mpString, that.mLength);
   mLength = that.mLength;
   mpString[mLength] = 0;
}

String::~String()
{
   free(mpString);
}

const UChar String::operator[](int index) const
{
   return mpString[index];
}

UChar String::operator[](int index)
{
   return mpString[index];
}

const String& String::operator=(const UChar c)
{
   wchar_t* pbuffer = getBuffer(1);
   pbuffer[0] = c;
   mLength = 1;
   return *this;
}

const String& String::operator=(const String& that)
{
   setTo(that.mpString, that.mLength);
   return *this;
}

const String& String::operator=(const UChar* pstring)
{
   setTo(pstring, wcslen(pstring));
   return *this;
}

const String& String::operator=(const std::string& that)
{
   setToUtf8(that.c_str(), that.length());
   return *this;
}

bool String::operator<=(const String& that) const
{
   return compare(that) <= 0;
}

bool String::operator<(const String& that) const
{
   return compare(that) < 0;
}

bool String::operator>(const String& that) const
{
   return compare(that) > 0;
}

bool String::operator>=(const String& that) const
{
   return compare(that) >= 0;
}

const String& String::operator+=(UChar c)
{
   wchar_t* pdata = getBuffer(mLength + 1);
   pdata[mLength++] = c;
   pdata[mLength] = 0;
   return *this;
}

const String& String::operator+=(const UChar* pdata)
{
   return *this;
}

const String& String::operator+=(const String& that)
{
   append(that);
   return *this;
}

String String::operator+(const String& that) const
{
   String result(*this);
   result += that;
   return result;
}

String String::operator+(UChar c) const
{
   String result(*this);
   result += c;
   return result;
}

CORE_API String operator+(const UChar* pleft, const String& right)
{
   return String(pleft) + right;
}

// - Query

bool String::isEmpty() const
{
   return mLength == 0;
}

uint32_t String::length() const
{
   return mLength;
}

int String::compare(const String& that) const
{
   if ( mLength == 0 && that.mLength == 0 )
   {
      return 0;
   }
   if ( mLength == 0 )
   {
      return -1;
   }
   else if ( that.mLength == 0 )
   {
      return 1;
   }

   return wcscmp(mpString, that.mpString);
}

int String::hashCode() const
{
   std::hash<std::string> str_hash;
   return str_hash(toUtf8()); // need to write it here
}

// - Comparison

bool String::operator==(const String& that) const
{
   return (mLength == that.mLength) && wcsncmp(mpString, that.mpString, mLength) == 0;
}

bool String::operator!=(const String& that) const
{
   return !operator==(that);
}

Object* String::clone() const
{
   return new String(*this);
}

bool String::equals(const Object& that) const
{
   const String& str = dynamic_cast<const String&>(that);
   return operator==(str);
}

String String::toString() const
{
   return *this;
}

// - Operations

UChar* String::getBuffer(uint32_t length)
{
   if ( mCapacity < length + 1 )
   {
      wchar_t* presult = (wchar_t*) realloc(mpString, (length + 1) * sizeof(wchar_t));
      if ( presult == NULL )
      {
         // no more memory!!
      }

      mpString = presult;
      mCapacity = length + 1;
   }

   return mpString;
}

const String& String::toLower()
{
    _wcslwr_s(mpString, mLength + 1);
   return *this;
}

const String& String::toUpper()
{
   _wcsupr_s(mpString, mLength + 1);
   return *this;
}

const String& String::trim()
{
   trimLeft();
   trimRight();
   return *this;
}

void String::trimLeft()
{
   uint32_t index = 0;
   for ( index = 0; index < mLength; ++index )
   {
      if ( !Char::isWhitespace(mpString[index]) )
      {
         break;
      }
   }

   if ( index > 0 )
   {
      // move the string to left
      mLength -= index;
      wmemmove(mpString, &mpString[index], mLength);
   }
}

void String::trimRight()
{
   int index = mLength - 1;
   for ( ; index >= 0; --index )
   {
      wchar_t c = mpString[index];
      if ( !Char::isWhitespace(c) )
      {
         break;
      }
   }

   mLength = index + 1;
   mpString[mLength] = 0;
}

void String::append(const String& that)
{
   wchar_t* pdata = getBuffer(mLength + that.mLength);
   wchar_t* pstart = &pdata[mLength];
   wmemmove(pstart, that.mpString, that.mLength);
   mLength += that.mLength;
   pdata[mLength] = 0;
}

void String::setTo(const UChar* ptext, uint32_t length)
{
   wchar_t* pbuffer = getBuffer(length);
   wcsncpy(pbuffer, ptext, length);
   pbuffer[length] = 0;
   mLength = length;
}

void String::insert(int index, const String& text)
{
   uint32_t newlen = mLength + text.length();
   wchar_t* pbuffer = getBuffer(newlen);

   wmemmove(&pbuffer[index + text.length()], &pbuffer[index], mLength - index); 
   wmemmove(&pbuffer[index], text.mpString, text.mLength);

   mLength += text.mLength;
   pbuffer[mLength] = 0;
}

void String::replace(UChar original, UChar newtext)
{
   for ( uint32_t index = 0; index < mLength; ++index )
   {
      wchar_t c = mpString[index];
      if ( c == original )
      {
         mpString[index] = newtext;
      }
   }
}

void String::replace(int start, int count, const String& with)
{
   wchar_t* ptemp = NULL;
   uint32_t remainder = wcslen(&mpString[start + count]);
   if ( remainder > 0 )
   {
      ptemp = new wchar_t[remainder];
      wmemcpy(ptemp, &mpString[start+count], remainder);
   }

   uint32_t newlen = mLength - count + with.mLength;
   wchar_t* pdata = getBuffer(newlen);
   wmemcpy(&mpString[start], with.mpString, with.mLength);

   if ( remainder > 0 )
   {
      wmemcpy(&mpString[start + with.mLength], ptemp, remainder);
   }

   mLength = newlen;
   mpString[newlen] = 0;
}

/// removes characters [start, start+count>.
void String::remove(int start, int count)
{
   uint32_t len = wcslen(&mpString[start + count]);
   wmemmove(&mpString[start], &mpString[start + count], len);
   mLength = start + len;
   mpString[mLength] = 0;
}

String String::subStr(int start, int count) const
{
   String result;
   result.setTo(&mpString[start], count);
   return result;
}

String String::unescape() const
{
   String result;
   uint32_t pos = 0;
   char digs[9]="\0\0\0\0\0\0\0\0";
   int dno = 0;
   wchar_t* pdata = result.getBuffer(mLength);
   for ( uint32_t index = 0; index < mLength; ++index )
   {
      if ( mpString[index] == '\\' )
      {
         UChar c = mpString[++index];
         switch ( c )
         {
         case L'u':
            index++;
            while ( hex_digit(mpString[index]) && dno < 4 )
            {
               digs[dno++] = mpString[index++];
            }

            if (dno > 0)
            {
               pdata[pos++] = (wchar_t) strtol(digs, NULL, 16);
            }
            break;

         default:
            pdata[pos++] = convertEscape(c);
            break;
         }
      }
      else
      {
         pdata[pos++] = mpString[index];
      }
   }
   result.mLength = pos;
   result.mpString[pos] = 0;
   return result;
}

// - Searching

int String::indexOf(const String& that) const
{
   wchar_t* ploc = wcsstr(mpString, that.mpString);
   return ploc == NULL ? -1 : ploc - mpString;
}

int String::indexOf(UChar character, int start) const
{
   wchar_t* ploc = wcschr(&mpString[start], character);
   return ploc == NULL ? -1 : ploc - mpString;
}

int String::lastIndexOf(UChar character) const
{
   wchar_t* ppos = wcsrchr(mpString, character);
   return ppos == NULL ? -1 : ppos - mpString;
}

int String::lastIndexOf(UChar character, int start, int end) const
{
   wchar_t* presult = NULL;
   wchar_t* ploc = &mpString[start];
   do
   {
      ploc = wcschr(ploc, character);
      if ( ploc != NULL && ploc < &mpString[end] )
      {
         presult = ploc;
         ploc++;
      }
   }
   while ( ploc != NULL && ploc < &mpString[end] );

   return presult == NULL ? -1 : presult - &mpString[start];
}

// - Conversion

std::string String::toUtf8() const
{
   std::string result;
   if ( mLength > 0 )
   {
      char* pdata = conv_utf16_to_utf8(mpString, mLength);
      result.assign(pdata);
      delete[] pdata;
   }
   return result;
}

void String::setToUtf8(const char* pdata, uint32_t length)
{
   wchar_t* pbuffer = getBuffer(length);
   mLength = conv_utf8_to_utf16(pbuffer, length, pdata, length);
}

std::wstring String::toUtf16() const
{
   return std::wstring(mpString, mLength);
}

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

#include <cwctype>
#include <string.h>
#include <functional>
#include <algorithm>

#include "core/defines.h"
#include "core/conv/numberconverter.h"

#include "utf.h"
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
   if ( pdata != nullptr )
   {
      result.setToUtf8(pdata, strlen(pdata));
   }
   return result;
}

String::String():
   mData()
{
}

String::String(const String& that) :
   mData(that.mData)
{
}

String::String(String&& that) :
   mData(std::move(that.mData))
{
}

String::String(UChar ch):
   mData(&ch, 1)
{
}

String::String(const UChar* pdata):
   mData(pdata)
{
}

String::String(const std::string& that):
   mData()
{
   setToUtf8(that);
}

String::String(const std::wstring& that):
   mData(that)
{
}

String::~String()
{
}

UChar String::operator[](int index) const
{
   return mData[index];
}

const String& String::operator=(const UChar c)
{
   mData = c;
   return *this;
}

const String& String::operator=(const String& that)
{
   mData = that.mData;
   return *this;
}

const String& String::operator=(String&& that)
{
   mData = std::move(that.mData);
   return *this;
}

const String& String::operator=(const UChar* pstring)
{
   mData.assign(pstring);
   return *this;
}

const String& String::operator=(const std::string& that)
{
   setToUtf8(that);
   return *this;
}

bool String::operator<=(const String& that) const
{
   return mData <= that.mData;
}

bool String::operator<(const String& that) const
{
   return mData < that.mData;
}

bool String::operator>(const String& that) const
{
   return mData > that.mData;
}

bool String::operator>=(const String& that) const
{
   return mData >= that.mData;
}

const String& String::operator+=(UChar c)
{
   mData += c;
   return *this;
}

const String& String::operator+=(const UChar* pdata)
{
   mData.append(pdata);
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

CORE_API String operator+(const UChar ch, const String& right)
{
   String result;
   result = ch;
   result += right;
   return result;
}

// - Query

bool String::isEmpty() const
{
   return mData.empty();
}

uint32_t String::length() const
{
   return mData.length();
}

int String::compare(const String& that) const
{
   return mData.compare(that.mData);
}

int String::hashCode() const
{
   std::hash<std::wstring> str_hash;
   return str_hash(mData); // need to write it here
}

// - Comparison

bool String::operator==(const String& that) const
{
   return mData == that.mData;
}

bool String::operator!=(const String& that) const
{
   return !operator==(that);
}

// - Operations

const String& String::toLower()
{
   std::transform(mData.begin(), mData.end(), mData.begin(), tolower);
   return *this;
}

const String& String::toUpper()
{
   std::transform(mData.begin(), mData.end(), mData.begin(), toupper);
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
   auto it = std::find_if_not(mData.begin(), mData.end(), Char::isWhitespace);
   if ( it != mData.end() )
   {
      mData.erase(mData.begin(), it);
   }
}

void String::trimRight()
{
   auto it = std::find_if_not(mData.rbegin(), mData.rend(), Char::isWhitespace);
   if ( it != mData.rend() )
   {
      mData.erase(it.base(), mData.end());
   }
}

void String::append(const String& that)
{
   mData.append(that.mData);
}

void String::assign(const UChar* ptext, std::size_t length)
{
   mData.assign(ptext, length);
}

void String::insert(int index, const String& text)
{
   mData.insert(index, text.mData);
}

void String::replace(UChar original, UChar newtext)
{
   std::replace(mData.begin(), mData.end(), original, newtext);
}

void String::replace(const String& original, const String& with)
{
   int index = indexOf(original);
   if ( index != -1 )
   {
      replace(index, original.length(), with);
   }
}

void String::replace(int start, int count, const String& with)
{
   mData.replace(start, count, with.mData);
}

/// removes characters [start, start+count>.
void String::remove(int start, int count)
{
   mData.erase(start, count);
}

void String::clear()
{
   mData.clear();
}

String String::subStr(int start, int count) const
{
   String result;
   result.mData = mData.substr(start, count);
   return result;
}

String String::left(int to) const
{
   return subStr(0, to);
}

String String::right(int from) const
{
   String result;
   result.mData = mData.substr(from, mData.length() - from);
   return result;
}

String String::unescape() const
{
   String result;
   char digs[9]= "\0\0\0\0\0\0\0\0";
   int dno = 0;

   result.mData.reserve(mData.length());

   for ( uint32_t index = 0; index < mData.length(); ++index )
   {
      UChar ch = mData[index];
      if ( ch == '\\' )
      {
         UChar c = mData[++index];
         switch ( c )
         {
         case L'u':
            index++;
            while ( isxdigit(mData[index]) && dno < 4 )
            {
               digs[dno++] = static_cast<char>(mData[index++]);
            }

            if (dno > 0)
            {
               result.mData += (wchar_t) strtol(digs, nullptr, 16);
            }
            break;

         default:
            result.mData += convertEscape(c);
            break;
         }
      }
      else
      {
         result.mData += ch;
      }
   }

   return result;
}

// - Arguments

String& String::arg(int arg, const String& value)
{
   String num;
   String search = L'{' + NumberConverter::getInstance().format(num, arg) + L'}';
   replace(search, value);
   return *this;
}

String& String::arg(int arg, int value)
{
   String num;
   NumberConverter& inst = NumberConverter::getInstance();
   String search = L'{' + inst.format(num, arg) + L'}';
   replace(search, inst.format(num, value));
   return *this;
}

// - Searching

std::size_t String::indexOf(const String& that, int start) const
{
   ASSERT(start >= 0 && start < mData.length());
   return mData.find(that.mData, start);
}

std::size_t String::indexOf(UChar character, int start) const
{
   return mData.find(character, start);
}

std::size_t String::lastIndexOf(UChar character) const
{
   return mData.rfind(character);
}

std::size_t String::lastIndexOf(UChar character, int start, int end) const
{
   auto index = mData.rfind(character, end);
   if ( index >= start )
   {
      return index;
   }
   return -1;
}

// - Conversion

std::string String::toUtf8() const
{
   if ( !mData.empty() )
   {
      return conv_wchar_to_utf8(mData);
   }
   return std::string();
}

void String::setToUtf8(const char* pdata, std::size_t length)
{
   mData = conv_utf8_to_wchar(pdata, length);
}

void String::setToUtf8(const std::string& value)
{
   mData = conv_utf8_to_wchar(value);
}

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

const String String::sEmpty;

const String& String::empty()
{
   return sEmpty;
}

String String::fromUtf8(const char* pdata)
{
   String result;
   result.setTo(pdata, strlen(pdata));
   return result;
}

String::String():
   mString()
{
}

String::String(const UChar* pdata):
   mString(pdata)
{
}

String::String(const std::string& that):
   mString()
{
   setTo(that.c_str(), that.length());
}

String::String(const std::wstring& that):
   mString(that.c_str())
{
}

String::String(bool nullterm, const UChar* pdata):
   mString(nullterm, pdata, -1)
{
}

String::String(const String& that):
   mString(that.mString)
{
}

String::~String()
{
}

const UChar String::operator[](int index) const
{
   return mString[index];
}

UChar String::operator[](int index)
{
   return mString[index];
}

const String& String::operator=(const UChar c)
{
   mString = c;
   return *this;
}

const String& String::operator=(const String& that)
{
   mString = that.mString;
   return *this;
}

const String& String::operator=(const UChar* pstring)
{
   mString.setTo(pstring, u_strlen(pstring));
   return *this;
}

const String& String::operator=(const std::string& that)
{
   mString = that.c_str();
   return *this;
}

bool String::operator<=(const String& that) const
{
   return mString.compare(that.mString) <= 0;
}

bool String::operator<(const String& that) const
{
   return mString.compare(that.mString) < 0;
}

bool String::operator>(const String& that) const
{
   return mString.compare(that.mString) > 0;
}

bool String::operator>=(const String& that) const
{
   return mString.compare(that.mString) >= 0;
}

const String& String::operator+=(UChar c)
{
   mString += c;
   return *this;
}

const String& String::operator+=(const char* pdata)
{
   mString += pdata;
   return *this;
}

const String& String::operator+=(const String& that)
{
   mString += that.mString;
   return *this;
}

String String::operator+(const String& that) const
{
   String result;
   result.mString = mString + that.mString;
   return result;
}

String String::operator+(UChar c) const
{
   String result;
   result.mString = mString + c;
   return result;
}

CORE_API String operator+(const UChar* pleft, const String& right)
{
   return String(pleft) + right;
}

// - Query

bool String::isEmpty() const
{
   return mString.isEmpty() == TRUE;
}

int String::length() const
{
   return mString.length();
}

int String::compare(const String& that) const
{
   return mString.compare(that.mString);
}

int String::hashCode() const
{
   return mString.hashCode();
}

// - Comparison

bool String::operator==(const String& that) const
{
   return (mString == that.mString) == TRUE;
}

bool String::operator!=(const String& that) const
{
   return (mString == that.mString) == FALSE;
}

// - Operations

const String& String::toLower()
{
   mString.toLower();
   return *this;
}

const String& String::toUpper()
{
   mString.toUpper();
   return *this;
}

const String& String::trim()
{
   mString.trim();
   return *this;
}

void String::append(const String& that)
{
   *this += that;
}

void String::setTo(const char* ptext, int length)
{
   StringPiece piece(ptext, length);
   mString = UnicodeString::fromUTF8(piece);
}

void String::replace(UChar original, UChar newtext)
{
   mString.findAndReplace(original, newtext);
}

void String::replace(int start, int length, const String& with)
{
   mString.replace(start, length, with.mString);
}

void String::remove(int start, int count)
{
   mString.remove(start, count);
}

String String::subStr(int start, int count) const
{
   String result;
   mString.extract(start, count, result.mString);
   return result;
}

String String::unescape() const
{
   String result;
   result.mString = mString.unescape();
   return result;
}

// - Searching

int String::indexOf(UChar character, int start) const
{
   return mString.indexOf(character, start);
}

int String::lastIndexOf(UChar character) const
{
   return mString.lastIndexOf(character);
}

int String::lastIndexOf(UChar character, int start, int end) const
{
   return mString.lastIndexOf(character, start, end);
}

// - Conversion

std::string String::toUtf8() const
{
   int size = mString.length();
   char* pdata = new char[size+1];
   memset(pdata, 0, size + 1);

   CheckedArrayByteSink sink(pdata, size);
   mString.toUTF8(sink);

   std::string result(pdata, size);
   delete[] pdata;
   return result;
}

std::wstring String::toUtf16() const
{
   UErrorCode error = U_ZERO_ERROR;
   int size = mString.length();
   UChar* pdata = new UChar[size];
   mString.extract(pdata, size, error);
   std::wstring result(pdata, size);
   delete[] pdata;
   return result;
}

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
#ifndef STRING_H_
#define STRING_H_

#include <unicode/unistr.h>
#include <string>

#include "core/core_base.h"

class CORE_API String
{
public:
   static const String& empty();
   static String fromUtf8(const char* pdata);

            String();
            String(const String& that);
   explicit String(const std::string& that);
   explicit String(const std::wstring& that);
            String(bool nullterm, const UChar* pdata);
            String(const UChar* pdata);
           ~String();

   const UChar operator[](int index) const;
         UChar operator[](int index);

   const String& operator=(const UChar c);
   const String& operator=(const String& that);
   const String& operator=(const UChar* pstring);
   const String& operator=(const std::string& that);

   bool operator<=(const String& that) const;
   bool operator<(const String& that) const;
   bool operator>(const String& that) const;
   bool operator>=(const String& that) const;

   bool operator==(const String& that) const;
   bool operator!=(const String& that) const;

   const String& operator+=(UChar c);
   const String& operator+=(const char* pdata);
   const String& operator+=(const String& that);
   String operator+(const String& that) const;
   String operator+(UChar c) const;

   CORE_API friend String operator+(const UChar* pleft, const String& right);

 // query
   bool isEmpty() const;
   int length() const;
   int compare(const String& that) const;
   int hashCode() const;

 // operations
   const String& toLower();
   const String& toUpper();
   const String& trim();

   void append(const String& that);
   void setTo(const char* ptext, int length);

   void replace(UChar original, UChar newtext);
   void replace(int start, int length, const String& with);
   void remove(int start, int count);

   String subStr(int start, int count) const;
   String unescape() const;

 // searching
   int indexOf(UChar character, int start = 0) const;
   int lastIndexOf(UChar character) const;
   int lastIndexOf(UChar character, int start, int end) const;

 // conversion
   std::string  toUtf8() const;
   std::wstring toUtf16() const;

private:
   friend class NumberConverter;

   static const String sEmpty;

   UnicodeString mString;
};

#define UTEXT(text) String(TRUE, L ## text)

#endif // STRING_H_

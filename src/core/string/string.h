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

#include <cstdint>
#include <string>

#include "core/system/object.h"
#include "core/core_base.h"

#include "char.h"

CORE_TEMPLATE template class CORE_API std::allocator<wchar_t>;
CORE_TEMPLATE template class CORE_API std::basic_string<wchar_t>;

class CORE_API String : public Object
{
public:
   static const String& empty();
   static String fromUtf8(const char* pdata);

            String();
            String(const String& that);
            String(String&& that);
   explicit String(UChar ch);
   explicit String(const UChar* pdata);
   explicit String(const std::string& that);
   explicit String(const std::wstring& that);
           ~String();

   const UChar operator[](int index) const;
         UChar operator[](int index);

   const String& operator=(const String& that);
   const String& operator=(String&& that);
   const String& operator=(const UChar c);
   const String& operator=(const UChar* pstring);
   const String& operator=(const std::string& that);

   bool operator<=(const String& that) const;
   bool operator<(const String& that) const;
   bool operator>(const String& that) const;
   bool operator>=(const String& that) const;

   bool operator==(const String& that) const;
   bool operator!=(const String& that) const;

   const String& operator+=(UChar c);
   const String& operator+=(const UChar* pdata);
   const String& operator+=(const String& that);
   String operator+(const String& that) const;
   String operator+(UChar c) const;

   CORE_API friend String operator+(const UChar ch, const String& right);
   CORE_API friend String operator+(const UChar* pleft, const String& right);

 // query
   bool isEmpty() const;
   uint32_t length() const;
   int compare(const String& that) const;
   int hashCode() const;

   const UChar* c_str() const {
      return mData.c_str();
   }

 // overrides
   virtual Object* clone() const override;
   virtual bool equals(const Object& that) const override;
   virtual String toString() const;

 // operations
   const String& toLower();
   const String& toUpper();
   const String& trim();

   void trimLeft();
   void trimRight();

   void append(const String& that);
   void assign(const UChar* ptext, std::size_t length);

   void insert(int index, const String& text);
   void replace(UChar original, UChar newtext);
   void replace(int start, int length, const String& with);
   void replace(const String& original, const String& with);
   void remove(int start, int count);
   void clear();

   String subStr(int start, int count) const;
   String left(int to) const;
   String right(int from) const;

   String unescape() const;

 // arguments
   String& arg(int arg, const String& value);
   String& arg(int arg, int value);

 // searching
   std::size_t indexOf(const String& that, int start = 0) const;
   std::size_t indexOf(UChar character, int start = 0) const;
   std::size_t lastIndexOf(UChar character) const;
   std::size_t lastIndexOf(UChar character, int start, int end) const;

 // conversion
   std::string  toUtf8() const;

   void setToUtf8(const char* pdata, std::size_t length = 0);
   void setToUtf8(const std::string& value);

private:
   friend class NumberConverter;
   
 // static data
   static const String sEmpty;

 // data
   std::wstring mData;
};

#define UTEXT(text) String(L ## text)

#endif // STRING_H_

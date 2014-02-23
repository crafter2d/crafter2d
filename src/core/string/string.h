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

class CORE_API String : public Object
{
public:
   static const String& empty();
   static String fromUtf8(const char* pdata);

            String();
            String(const String& that);
   explicit String(const UChar* pdata);
   explicit String(const std::string& that);
   explicit String(const std::wstring& that);
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
   const String& operator+=(const UChar* pdata);
   const String& operator+=(const String& that);
   String operator+(const String& that) const;
   String operator+(UChar c) const;

   CORE_API friend String operator+(const UChar* pleft, const String& right);

 // query
   bool isEmpty() const;
   uint32_t length() const;
   int compare(const String& that) const;
   int hashCode() const;

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
   void setTo(const UChar* ptext, uint32_t length);

   void insert(int index, const String& text);
   void replace(UChar original, UChar newtext);
   void replace(int start, int length, const String& with);
   void remove(int start, int count);

   String subStr(int start, int count) const;
   String unescape() const;

 // searching
   int indexOf(const String& that) const;
   int indexOf(UChar character, int start = 0) const;
   int lastIndexOf(UChar character) const;
   int lastIndexOf(UChar character, int start, int end) const;

 // conversion
   std::string  toUtf8() const;
   std::wstring toUtf16() const;

   void setToUtf8(const char* pdata, uint32_t length);

private:
   friend class NumberConverter;

 // operations
   UChar* getBuffer(uint32_t length);

 // static data
   static const String sEmpty;

 // data
   UChar*   mpString;
   uint32_t mCapacity;
   uint32_t mLength;   // not including the eos character
};

#define UTEXT(text) String(L ## text)

#endif // STRING_H_

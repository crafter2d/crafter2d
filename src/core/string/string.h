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
            String();
            String(const String& that);
   explicit String(const std::string& that);
            String(const char* pdata);
           ~String();

   const char operator[](int index) const;
         char operator[](int index);

   const String& operator=(const char c);
   const String& operator=(const String& that);
   const String& operator=(const char* pstring);
   const String& operator=(const std::string& that);

   bool operator<=(const String& that) const;
   bool operator<(const String& that) const;
   bool operator>(const String& that) const;
   bool operator>=(const String& that) const;

   bool operator==(const String& that) const;
   bool operator!=(const String& that) const;

   const String& operator+=(char c);
   const String& operator+=(const String& that);
   String operator+(const String& that) const;
   String operator+(char c) const;

   CORE_API friend String operator+(const char* pleft, const String& right);

 // query
   bool isEmpty() const;
   int length() const;
   const char* getBuffer() const;
   char* toUtf8(int& length) const;
   int compare(const String& that) const;
   int hashCode() const;

 // operations
   const String& toLower();
   const String& toUpper();
   const String& trim();

   void append(const String& that);
   void setTo(const char* ptext, int length);

   void replace(char original, char newtext);
   void replace(int start, int length, const String& with);
   void remove(int start, int count);

   String subStr(int start, int count) const;
   String unescape() const;

 // searching
   int indexOf(char character, int start = 0) const;
   int lastIndexOf(char character) const;
   int lastIndexOf(char character, int start, int end) const;

 // conversion
   std::string toStdString() const;

private:
   friend class NumberConverter;

   UnicodeString mString;
};

#endif // STRING_H_

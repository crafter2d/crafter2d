
#ifndef STRING_LIST_H
#define STRING_LIST_H

#include <list>

#include "core/core_base.h"

#include "string.h"

class CORE_API StringList
{
public:
   StringList();
   
   typedef std::list<String>::iterator iterator;
   typedef std::list<String>::const_iterator const_iterator;

 // query
   bool isEmpty() const;
   bool contains(const String& str) const;
   int size() const;

   iterator begin() {
      return mStrings.begin();
   }
   const_iterator begin() const {
      return mStrings.begin();
   }

 // operations
   void add(const String& str);
   void clear();

private:
   typedef std::list<String> Strings;

 // data
   Strings mStrings;
};

#endif // STRING_LIST_H

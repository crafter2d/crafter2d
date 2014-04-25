
#ifndef STRING_LIST_H
#define STRING_LIST_H

#include "core/containers/list.h"
#include "core/core_base.h"

#include "string.h"

class CORE_API StringList
{
public:
   StringList();

 // query
   bool isEmpty() const;
   bool contains(const String& str) const;
   int size() const;

   ListIterator<String> getFront();
   ListConstIterator<String> getFront() const;

 // operations
   void add(const String& str);
   void clear();

private:
   typedef List<String> Strings;

 // data
   Strings mStrings;
};

#endif // STRING_LIST_H

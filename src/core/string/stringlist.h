
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
   bool contains(const String& str) const;

   ListIterator<String> getFront();

 // operations
   void add(const String& str);

private:
   typedef List<String> Strings;

 // data
   Strings mStrings;
};

#endif // STRING_LIST_H

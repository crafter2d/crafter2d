
#ifndef CLASS_RESOLVER_H_
#define CLASS_RESOLVER_H_

#include <set>

#include "core/string/string.h"

class ClassResolver
{
   typedef std::set<String> Paths;

public:
   ClassResolver();

   const ClassResolver& operator=(const ClassResolver& that);

 // operations
   void insert(const String& path);

   String resolve(const String& classname) const;

private:
   bool checkClassExists(const String& classname) const;

   Paths mPaths;
};

#endif // CLASS_RESOLVER_H_

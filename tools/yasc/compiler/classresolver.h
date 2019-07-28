
#ifndef CLASS_RESOLVER_H_
#define CLASS_RESOLVER_H_

#include <set>

#include "core/string/string.h"

class ClassResolver
{
public:
   ClassResolver();
   ClassResolver(const ClassResolver& that) = delete;
   ClassResolver(ClassResolver&& that);
   ClassResolver& operator=(const ClassResolver& that) = delete;
   ClassResolver& operator=(ClassResolver&& that);

 // operations
   void insert(const String& path);
   String resolve(const String& classname) const;

private:
   using Paths = std::set<String>;

 // operations
   bool checkClassExists(const String& classname) const;

 // members
   Paths mPaths;
};

#endif // CLASS_RESOLVER_H_

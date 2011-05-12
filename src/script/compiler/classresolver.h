
#ifndef CLASS_RESOLVER_H_
#define CLASS_RESOLVER_H_

#include <string>
#include <set>

class ClassResolver
{
   typedef std::set<std::string> Paths;

public:
   ClassResolver();

 // operations
   void insert(const std::string& path);

   std::string resolve(const std::string& classname) const;

private:
   bool checkFileExists(const std::string& file) const;

   Paths mPaths;
};

#endif // CLASS_RESOLVER_H_

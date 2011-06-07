
#ifndef CLASS_RESOLVER_H_
#define CLASS_RESOLVER_H_

#include <string>
#include <vector>

class ClassResolver
{
   typedef std::vector<std::string> Paths;

public:
   ClassResolver();

   const ClassResolver& operator=(const ClassResolver& that);

 // operations
   void insert(const std::string& path);

   std::string resolve(const std::string& classname) const;

private:
   bool checkClassExists(const std::string& classname) const;

   Paths mPaths;
};

#endif // CLASS_RESOLVER_H_

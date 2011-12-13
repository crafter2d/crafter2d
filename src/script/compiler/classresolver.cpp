
#include "classresolver.h"

#include "core/vfs/filesystem.h"
#include "core/string/string.h"

using namespace std;

ClassResolver::ClassResolver():
   mPaths()
{
   mPaths.push_back("*");
   mPaths.push_back("System.*");
}

const ClassResolver& ClassResolver::operator=(const ClassResolver& that)
{
   mPaths = that.mPaths;
   return *this;
}

// - Operations

void ClassResolver::insert(const std::string& path)
{
   std::string qualifiedpath = path;
   if ( path.find('*') == std::string::npos )
   {
      // no * so replace last part with *
      std::size_t pos = path.rfind('.');
      if ( pos == std::string::npos )
      {
         return;
      }
      else
      {
         qualifiedpath.replace(pos+1, path.length(), "*");
      }
   }

   mPaths.push_back(qualifiedpath);
}

std::string ClassResolver::resolve(const std::string& classname) const
{
   String s = String(classname.c_str()).toLower();
   string j = s.toStdString();

   for ( int index = 0; index < mPaths.size(); ++index )
   {
      const string& path = mPaths[index];

      size_t pos = path.rfind('*');
      if ( pos != string::npos )
      {
         string fullclassname = path.substr(0, pos) + classname;
         if ( checkClassExists(fullclassname) )
         {
            return fullclassname;
         }
      }
      else if ( path.compare(j) == 0 )
      {
         return classname;
      }
   }

   return "";
}

#include "core/string/string.h"

bool ClassResolver::checkClassExists(const string& classname) const
{
   String name = classname.c_str();
   name.replace('.', '/');

   return FileSystem::getInstance().exists(name.toStdString() + ".as");
}


#include "classresolver.h"

#include "core/vfs/filesystem.h"

using namespace std;

ClassResolver::ClassResolver():
   mPaths()
{
}

// - Operations

void ClassResolver::insert(const std::string& path)
{
   mPaths.push_back(path);
}

std::string ClassResolver::resolve(const std::string& classname) const
{
   for ( int index = 0; index < mPaths.size(); ++index )
   {
      const string& path = mPaths[index];

      size_t pos = path.rfind('.');
      if ( pos != string::npos )
      {
         string filepart = path.substr(pos+1, path.length());
         if ( filepart == "*" )
         {
            string fullclassname = path.substr(0, pos + 1) + classname;
            if ( checkClassExists(fullclassname) )
            {
               return fullclassname;
            }
         }
         else if ( filepart.compare(classname) == 0 )
         {
            return path;
         }
      }
      else if ( path.compare(classname) == 0 )
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

   return FileSystem::getInstance().exists("ascripts/" + name.toStdString() + ".as");
}

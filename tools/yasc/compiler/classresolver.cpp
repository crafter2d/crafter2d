
#include "classresolver.h"

#include "core/vfs/filesystem.h"
#include "core/string/string.h"

using namespace std;

ClassResolver::ClassResolver():
   mPaths()
{
   mPaths.insert(UTEXT("*"));
   mPaths.insert(UTEXT("system.*"));
}

const ClassResolver& ClassResolver::operator=(const ClassResolver& that)
{
   mPaths = that.mPaths;
   return *this;
}

// - Operations

void ClassResolver::insert(const String& path)
{
   String qualifiedpath = path;
   if ( path.indexOf('*') == String::npos )
   {
      // no * so replace last part with *
      std::size_t pos = path.lastIndexOf('.');
      if ( pos == String::npos )
      {
         return;
      }
      else
      {
         qualifiedpath.replace(pos+1, path.length() - (pos + 1), UTEXT("*"));
      }
   }

   mPaths.insert(qualifiedpath);
}

String ClassResolver::resolve(const String& classname) const
{
   String lowerclassname(classname);
   lowerclassname.toLower();

   for ( auto& path : mPaths )
   {
      std::size_t pos = path.lastIndexOf('*');
      if ( pos != String::npos )
      {
         String fullclassname = path.subStr(0, pos) + classname;
         if ( checkClassExists(fullclassname) )
         {
            return fullclassname;
         }
      }
      else if ( path.compare(lowerclassname) == 0 )
      {
         return classname;
      }
   }

   return String::empty();
}

bool ClassResolver::checkClassExists(const String& classname) const
{
   String name(classname);
   name.replace('.', '/');

   return FileSystem::getInstance().exists(name + UTEXT(".as"));
}

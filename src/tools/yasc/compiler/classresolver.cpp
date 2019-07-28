
#include "classresolver.h"

#include "core/defines.h"
#include "core/vfs/filesystem.h"
#include "core/string/string.h"

using namespace std;

static UChar sSTAR{ '*' };
static UChar sDOT{ '.' };

ClassResolver::ClassResolver():
   mPaths()
{
   mPaths.insert(UTEXT(""));
   mPaths.insert(UTEXT("system."));
}

ClassResolver::ClassResolver(ClassResolver&& that):
   mPaths(std::move(that.mPaths))
{
}

ClassResolver& ClassResolver::operator=(ClassResolver&& that)
{
   mPaths = std::move(that.mPaths);
   return *this;
}

// - Operations

void ClassResolver::insert(const String& path)
{
   if ( path.isEmpty() )
      return;

   std::size_t pos = String::npos;
   if ( path[path.length() - 1] == sSTAR )
   {
      ASSERT(path[path.length() - 2] == sDOT);
      pos = path.length() - 2;
   }
   else
   {
      pos = path.lastIndexOf('.');
   }

   if ( pos != String::npos )
   {
      String qualifiedpath = path.subStr(0, pos + 1);
      mPaths.insert(std::move(qualifiedpath));
   }
}

String ClassResolver::resolve(const String& classname) const
{
   for ( auto& path : mPaths )
   {    
      String fullclassname = path + classname;
      if ( checkClassExists(fullclassname) )
      {
         return fullclassname;
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

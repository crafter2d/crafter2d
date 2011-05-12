
#include "classresolver.h"

using namespace std;

ClassResolver::ClassResolver():
   mPaths()
{
   mPaths.insert("Object");
   mPaths.insert("InternalArray");
   mPaths.insert("ClassLoader");
   mPaths.insert("System");
}

// - Operations

void ClassResolver::insert(const std::string& path)
{
   mPaths.insert(path);
}

std::string ClassResolver::resolve(const std::string& classname) const
{
   Paths::iterator it = mPaths.begin();
   for ( ; it != mPaths.end(); it++ )
   {
      const string& path = *it;

      size_t pos = path.rfind('/');
      if ( pos != string::npos )
      {
         string filepart = path.substr(pos+1, path.length());
         if ( filepart == "*" )
         {
            std::string file = path.substr(0, pos + 1) + classname;
            if ( checkFileExists(file + ".as") )
            {
               return file;
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

bool ClassResolver::checkFileExists(const string& file) const
{
   return true;
}

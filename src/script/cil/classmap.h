
#ifndef CIL_CLASS_MAP_H
#define CIL_CLASS_MAP_H

#include <map>

#include "core/string/string.h"

namespace CIL
{
   class Class;

   class ClassMap
   {
      typedef std::map<String, Class*> Classes;

   public:

      void insert(Class* pclass);

   private:
      Classes mClassMap;

   };
}

#endif // CIL_CLASS_MAP_H

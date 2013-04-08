
#include "class.h"

namespace CIL
{

   Class::Class():
      mFunctions(),
      mName(),
      mBaseName(),
      mModifiers(0)
   {
   }

   // - Get/set

   void Class::setName(const String& name)
   {
      mName = name;
   }
      
   void Class::setBaseName(const String& name)
   {
      mBaseName = name;
   }
      
   void Class::setModifiers(int modifiers)
   {
      mModifiers = modifiers;
   }

   // - Operations
   
   void Class::addFunction(Function* pfunction)
   {
      mFunctions.push_back(pfunction);
   }

} // namespace CIL

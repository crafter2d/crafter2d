
#include "class.h"

namespace CIL
{

   Class::Class():
      mFunctions(),
      mInterfaces(),
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

   // - Query

   const Class::Functions& Class::getFunctions() const
   {
      return mFunctions;
   }

   // - Operations
   
   void Class::addInterface(const String& interfce)
   {
      mInterfaces.push_back(interfce);
   }

   void Class::addFunction(Function* pfunction)
   {
      mFunctions.push_back(pfunction);
   }

} // namespace CIL

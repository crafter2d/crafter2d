
#include "class.h"

#include "field.h"
#include "function.h"

namespace CIL
{
   Class::Class():
      mFunctions(),
      mName(),
      mBaseName(),
      mInterfaces(),
      mModifiers(),
      mFields(),
      mStatics()
   {
   }

   // - Get/set

   const String& Class::getName() const
   {
      return mName;
   }

   void Class::setName(const String& name)
   {
      mName = name;
   }

   const String& Class::getBaseName() const
   {
      return mBaseName;
   }

   void Class::setBaseName(const String& name)
   {
      mBaseName = name;
   }
   
   Modifiers& Class::getModifiers()
   {
      return mModifiers;
   }

   const Class::Functions& Class::getFunctions() const
   {
      return mFunctions;
   }

   Class::Functions& Class::getFunctions()
   {
      return mFunctions;
   }

   const Class::Fields& Class::getFields() const
   {
      return mFields;
   }

   Class::Fields& Class::getFields()
   {
      return mFields;
   }

   const Class::Fields& Class::getStatics() const
   {
      return mStatics;
   }

   Class::Fields& Class::getStatics()
   {
      return mStatics;
   }

   const Class::Interfaces& Class::getInterfaces() const
   {
      return mInterfaces;
   }

   // - Operations

   void Class::add(Function* pfunction)
   {
      mFunctions.push_back(pfunction);
   }

   void Class::addField(Field* pfield)
   {
      mFields.push_back(pfield);
   }
    
   void Class::addStatic(Field* pfield)
   {
      mStatics.push_back(pfield);
   }

   void Class::addInterface(const String& name)
   {
      mInterfaces.push_back(name);
   }
} // namespace CIL

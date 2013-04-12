
#include "class.h"

namespace CIL
{

   Class::Class():
      mFunctions(),
      mInterfaces(),
      mFields(),
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

   const Class::Fields& Class::getFields() const
   {
      return mFields;
   }

   const Class::Fields& Class::getStaticFields() const
   {
      return mStatics;
   }

   // - Operations

   void Class::addField(CIL::Type* pfield)
   {
      mFields.push_back(pfield);
   }

   void Class::addStaticField(CIL::Type* pfield)
   {
      mStatics.push_back(pfield);
   }
   
   void Class::addInterface(const String& interfce)
   {
      mInterfaces.push_back(interfce);
   }

   void Class::addFunction(Function* pfunction)
   {
      mFunctions.push_back(pfunction);
   }

} // namespace CIL

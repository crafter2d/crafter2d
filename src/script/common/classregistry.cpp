
#include "classregistry.h"

#include "core/defines.h"

#include "script/ast/astclass.h"
#include "script/ast/astfunction.h"

#include "classregistration.h"
#include "functionregistration.h"

ClassRegistry::ClassRegistry():
   mClasses(),
   mFunctions(),
   mpCurrent(NULL)
{
}

VMInterface::CallbackFnc ClassRegistry::getCallback(int index) const
{
   return mFunctions[index]->getCallback();
}

const FunctionRegistration& ClassRegistry::getFunction(int index) const
{
   return *mFunctions[index];
}

// - Maintenance

void ClassRegistry::add(const ClassRegistry& that)
{
   for ( std::size_t index = 0; index < that.mClasses.size(); ++index )
   {
      const ClassRegistration& thatreg = *that.mClasses[index];

      ClassRegistration* reg = new ClassRegistration();
      reg->name = thatreg.name;
      reg->start = thatreg.start + mFunctions.size();
      reg->end = thatreg.end + mFunctions.size();

      mClasses.push_back(reg);
   }

   for ( std::size_t index = 0; index < that.mFunctions.size(); ++index )
   {
      const FunctionRegistration* preg = that.mFunctions[index];

      mFunctions.push_back(preg->clone());
   }
}

void ClassRegistry::addClass(const String& name)
{
   ClassRegistration* reg = new ClassRegistration();
   reg->name = name;
   reg->start = mFunctions.size();
   reg->end = reg->start;

   mClasses.push_back(reg);
   mpCurrent = reg;
}

void ClassRegistry::addFunction(const String& name, VMInterface::CallbackFnc callback)
{
   FunctionRegistration* pregistration = FunctionRegistration::create(name, callback);
   pregistration->setClassRegistration(*mpCurrent);
   pregistration->setIndex(mFunctions.size());
   mFunctions.push_back(pregistration);

   mpCurrent->end++;
}

// - Search

const ClassRegistration* ClassRegistry::findClass(const String& name) const
{
   return const_cast<ClassRegistry*>(this)->findClass(name);
}

ClassRegistration* ClassRegistry::findClass(const String& name)
{
   for ( std::size_t index = 0; index < mClasses.size(); index++ )
   {
      ClassRegistration& reg = *mClasses[index];
      if ( reg.name == name )
      {
         return &reg;
      }
   }
   return NULL;
}

const FunctionRegistration* ClassRegistry::findCallback(const ASTClass& astclass, const ASTFunction& function) const
{
   return findCallback(astclass, function.getPrototype());
}

const FunctionRegistration* ClassRegistry::findCallback(const ASTClass& astclass, const String& fncname) const
{
   const ClassRegistration* pclass = findClass(astclass.getFullName());
   if ( pclass != NULL )
   {
      for ( int index = pclass->start; index < pclass->end; ++index )
      {
         const FunctionRegistration* pfuncreg = mFunctions[index];
         if ( pfuncreg->getPrototype() == fncname )
         {
            return pfuncreg;
         }
      }
   }

   return NULL;
}

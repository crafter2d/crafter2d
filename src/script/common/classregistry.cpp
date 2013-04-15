
#include "classregistry.h"

#include "core/defines.h"

#include "script/ast/astclass.h"
#include "script/ast/astfunction.h"

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
      const ClassRegistration& thatreg = that.mClasses[index];

      ClassRegistration reg;
      reg.start = thatreg.start + mFunctions.size();
      reg.end = thatreg.end + mFunctions.size();

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
   ClassRegistration reg;
   reg.name = name;
   reg.start = mFunctions.size();
   reg.end = reg.start;

   mClasses.push_back(reg);
   mpCurrent = &reg;
}

void ClassRegistry::addFunction(const String& name, VMInterface::CallbackFnc callback)
{
   mFunctions.push_back(FunctionRegistration::create(name, callback));
   mpCurrent->end++;
}

// - Search

const ClassRegistry::ClassRegistration* ClassRegistry::findClass(const String& name) const
{
   return const_cast<ClassRegistry*>(this)->findClass(name);
}

ClassRegistry::ClassRegistration* ClassRegistry::findClass(const String& name)
{
   for ( std::size_t index = 0; index < mClasses.size(); index++ )
   {
      ClassRegistration& reg = mClasses[index];
      if ( reg.name == name )
      {
         return &reg;
      }
   }
   return NULL;
}

const FunctionRegistration* ClassRegistry::findCallback(const ASTClass& astclass, const ASTFunction& function) const
{
   return findCallback(astclass, function.getName());
}

const FunctionRegistration* ClassRegistry::findCallback(const ASTClass& astclass, const String& fncname) const
{
   const ClassRegistration* pclass = findClass(astclass.getName());
   if ( pclass != NULL )
   {
      for ( std::size_t index = pclass->start; index < pclass->end; ++index )
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

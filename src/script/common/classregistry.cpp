
#include "classregistry.h"

#include "core/defines.h"

#include "script/ast/astclass.h"
#include "script/ast/astfunction.h"

#include "classregistration.h"
#include "functionregistration.h"

void ClassRegistry::assign(const ClassRegistry& that)
{
   mClasses = that.mClasses;
   renumber();
}

ClassRegistration& ClassRegistry::addClass(const String& name)
{
   mClasses.push_back(new ClassRegistration(name));
   return *mClasses[mClasses.size() - 1];
}

void ClassRegistry::merge(const ClassRegistry& that)
{
   for ( std::size_t index = 0; index < that.mClasses.size(); index++ )
   {
      const ClassRegistration& classreg = *that.mClasses[index];

      ClassRegistration* thisclassreg = findClass(classreg.getName());
      if ( thisclassreg != NULL )
      {
         thisclassreg->merge(classreg);
      }
      else
      {
         mClasses.push_back(classreg.clone());
      }
   }

   renumber();
}

void ClassRegistry::renumber()
{
   int index = 0;
   for ( std::size_t idx = 0; idx < mClasses.size(); idx++ )
   {
      ClassRegistration& reg = *mClasses[idx];
      reg.renumber(index);
   }
}

void ClassRegistry::collect(std::vector<VMInterface::CallbackFnc>& callbacks) const
{
   for ( std::size_t index = 0; index < mClasses.size(); index++ )
   {
      const ClassRegistration& reg = *mClasses[index];
      reg.collect(callbacks);
   }
}

// search

const ClassRegistration* ClassRegistry::findClass(const String& name) const
{
   return const_cast<ClassRegistry*>(this)->findClass(name);
}

ClassRegistration* ClassRegistry::findClass(const String& name)
{
   for ( std::size_t index = 0; index < mClasses.size(); index++ )
   {
      ClassRegistration* pregistration = mClasses[index];
      if ( pregistration->getName() == name )
      {
         return pregistration;
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
      const FunctionRegistration* funcreg = pclass->find(fncname);
      if ( funcreg != NULL )
      {
         return funcreg;
      }
   }

   return NULL;
}
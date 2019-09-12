
#include "classregistry.h"

#include <memory>

#include "core/defines.h"

#include "classregistration.h"
#include "functionregistration.h"

ClassRegistry::ClassRegistry():
   mClasses(),
   mFunctions(),
   mpCurrent(nullptr)
{
}

ClassRegistry::~ClassRegistry()
{
   clear();
}

ClassRegistry& ClassRegistry::operator=(ClassRegistry&& that)
{
   clear();
   add(std::move(that));
   mpCurrent = nullptr;
   return *this;
}

yas::CallbackFunctor& ClassRegistry::getFunctionCallback(int index) const
{
   return mFunctions[index]->getCallback();
}

const FunctionRegistration& ClassRegistry::getFunction(int index) const
{
   return *mFunctions[index];
}

// - Maintenance

void ClassRegistry::add(ClassRegistry&& that)
{
   std::move(that.mClasses.begin(), that.mClasses.end(), std::back_inserter(mClasses));
   std::move(that.mFunctions.begin(), that.mFunctions.end(), std::back_inserter(mFunctions));

   that.mClasses.clear();
   that.mFunctions.clear();
   that.mpCurrent = nullptr;

   renumber();
}

void ClassRegistry::addClass(const String& name)
{
   mClasses.emplace_back(name, mFunctions.size(), mFunctions.size());
   mpCurrent = &mClasses.back();
}

void ClassRegistry::addFunction(const String& prototype, yas::CallbackFunctor* pcallback)
{
   auto pregistration = std::make_unique<FunctionRegistration>(prototype, pcallback);
   pregistration->setClassRegistration(*mpCurrent);
   pregistration->setIndex(mFunctions.size());

   mpCurrent->addFunction(*pregistration);
   mFunctions.push_back(pregistration.release());
}

void ClassRegistry::clear()
{
   mClasses.clear();
   mFunctions.clear();
}

// - Operations

void ClassRegistry::renumber()
{
   for ( std::size_t index = 0; index < mFunctions.size(); ++index )
   {
      mFunctions[index]->setIndex(index);
   }
}

// - Search

const ClassRegistration* ClassRegistry::findClass(const String& name) const
{
   return const_cast<ClassRegistry*>(this)->findClass(name);
}

ClassRegistration* ClassRegistry::findClass(const String& name)
{
   for ( ClassRegistration& registration : mClasses)
   {
      if (registration.mName == name )
      {
         return &registration;
      }
   }
   return nullptr;
}

FunctionRegistration* ClassRegistry::findCallback(const String& qualifiedname)
{
   std::size_t indexB = qualifiedname.indexOf(L'(');
   std::size_t index = qualifiedname.lastIndexOf(L'.', 0, indexB);
   ASSERT(indexB != String::npos && index != String::npos);

   String klass = qualifiedname.subStr(0, index);
   String function = qualifiedname.subStr(index + 1, qualifiedname.length() - index - 1);

   //012345678901234567890123456789
   //system.InternalString.length()

   const ClassRegistration* pclass = findClass(klass);
   if ( pclass )
   {
      for ( FunctionRegistration* pfunction : pclass->mFunctions )
      {
         if ( pfunction->getPrototype() == function )
         {
            return pfunction;
         }
      }
   }
   return nullptr;
}

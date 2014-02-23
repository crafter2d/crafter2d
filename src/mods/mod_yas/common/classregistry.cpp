
#include "classregistry.h"

#include "core/defines.h"

#include "classregistration.h"
#include "functionregistration.h"

ClassRegistry::ClassRegistry():
   mClasses(),
   mFunctions(),
   mpCurrent(NULL)
{
}

ClassRegistry::~ClassRegistry()
{
   clear();
}

ClassRegistry& ClassRegistry::operator=(const ClassRegistry& that)
{
   clear();
   add(that);
   return *this;
}

yas::CallbackFunctor& ClassRegistry::getCallback(int index) const
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

   renumber();
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

void ClassRegistry::addFunction(const String& name, yas::CallbackFunctor* pcallback)
{
   FunctionRegistration* pregistration = FunctionRegistration::create(name, pcallback);
   pregistration->setClassRegistration(*mpCurrent);
   pregistration->setIndex(mFunctions.size());
   mFunctions.push_back(pregistration);

   mpCurrent->functions.push_back(pregistration);
   mpCurrent->end++;
}

void ClassRegistry::clear()
{
   for ( int index = 0; index < mClasses.size(); ++index )
   {
      ClassRegistration* reg = mClasses[index];
      delete reg;
   }

   for ( int index = 0; index < mFunctions.size(); ++index )
   {
      FunctionRegistration* pfunc = mFunctions[index];
      delete pfunc;
   }

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

const FunctionRegistration* ClassRegistry::findCallback(const String& qualifiedname) const
{
   int indexB = qualifiedname.indexOf(L'(');
   int index = qualifiedname.lastIndexOf(L'.', 0, indexB);
   String klass = qualifiedname.subStr(0, index);
   String function = qualifiedname.subStr(index + 1, qualifiedname.length() - index - 1);

   //012345678901234567890123456789
   //system.InternalString.length()

   const ClassRegistration* pclass = findClass(klass);
   if ( pclass != NULL )
   {
      for ( int index = pclass->start; index < pclass->end; ++index )
      {
         const FunctionRegistration* pfuncreg = mFunctions[index];
         if ( pfuncreg->getPrototype() == function )
         {
            return pfuncreg;
         }
      }
   }
   return NULL;
}


#include "classregistration.h"

#include "functionregistration.h"

ClassRegistration::ClassRegistration(const String& name):
   mName(name),
   mConstructors(),
   mDestructors(),
   mMembers()
{
}

ClassRegistration::ClassRegistration(const ClassRegistration& that):
   mName(that.mName),
   mConstructors(),
   mDestructors(),
   mMembers()
{
   copyTo(mConstructors, that.mConstructors);
   copyTo(mDestructors, that.mDestructors);
   copyTo(mMembers, that.mMembers);
}

const ClassRegistration& ClassRegistration::operator=(const ClassRegistration& that)
{
   mName = that.mName;
   copyTo(mConstructors, that.mConstructors);
   copyTo(mDestructors, that.mDestructors);
   copyTo(mMembers, that.mMembers);
   return *this;
}

// - Get/set
const String& ClassRegistration::getName() const
{
   return mName;
}

void ClassRegistration::addFunction(const FunctionRegistration& function)
{
   switch ( function.getType() )
   {
   case FunctionRegistration::eConstructor:
      mConstructors.push_back(function.clone());
      break;
   case FunctionRegistration::eDestructor:
      mDestructors.push_back(function.clone());
      break;
   case FunctionRegistration::eFunction:
      mMembers.push_back(function.clone());
      break;
   }
}

void ClassRegistration::renumber(int& startindex)
{
   renumber(startindex, mConstructors);
   renumber(startindex, mDestructors);
   renumber(startindex, mMembers);
}

void ClassRegistration::renumber(int& startindex, Functions& functions)
{
   for ( std::size_t index = 0; index < functions.size(); index++ )
   {
      FunctionRegistration& functionreg = *functions[index];
      functionreg.setIndex(startindex++);
   }
}

void ClassRegistration::merge(const ClassRegistration& that)
{
   merge(mConstructors, that.mConstructors);
   merge(mDestructors , that.mDestructors);
   merge(mMembers     , that.mMembers);
}

void ClassRegistration::merge(Functions& dst, const Functions& src)
{
   for ( std::size_t index = 0; index < src.size(); index++ )
   {
      const FunctionRegistration& functionreg = *src[index];

      bool found = false;
      for ( std::size_t destidx = 0; destidx < dst.size(); destidx++ )
      {
         const FunctionRegistration& destreg = *dst[destidx];
         if ( destreg.getPrototype() == functionreg.getPrototype() )
         {
            found = true;
            break;
         }
      }

      if ( !found )
      {
         dst.push_back(functionreg.clone());
      }
   }
}

void ClassRegistration::collect(std::vector<VMInterface::CallbackFnc>& callbacks) const
{
   collectFrom(callbacks, mConstructors);
   collectFrom(callbacks, mDestructors);
   collectFrom(callbacks, mMembers);
}

void ClassRegistration::collectFrom(std::vector<VMInterface::CallbackFnc>& callbacks, const Functions& source) const
{
   for ( std::size_t index = 0; index < source.size(); ++index )
   {
      const FunctionRegistration& functionreg = *source[index];
      callbacks.push_back(functionreg.getCallback());
   }
}

ClassRegistration* ClassRegistration::clone() const
{
   return new ClassRegistration(*this);
}

void ClassRegistration::copyTo(Functions& dest, const Functions& source)
{
   for ( std::size_t index = 0; index < source.size(); ++index )
   {
      const FunctionRegistration& functionreg = *source[index];

      dest.push_back(functionreg.clone());
   }
}

// - Search

const FunctionRegistration* ClassRegistration::find(const String& name) const
{
   const FunctionRegistration* preg = find(mMembers, name);
   if ( preg == NULL )
   {
      preg = find(mConstructors, name);
      if ( preg == NULL )
      {
         preg = find(mDestructors, name);
      }
   }
   return preg;
}

const FunctionRegistration* ClassRegistration::find(const Functions& functions, const String& name) const
{
   for ( std::size_t index = 0; index < functions.size(); index++ )
   {
      const FunctionRegistration* pfunctionreg = functions[index];
      if ( pfunctionreg->getPrototype() == name )
      {
         return pfunctionreg;
      }
   }

   return NULL;
}
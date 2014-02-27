
#include "virtualclasses.h"

#include "virtualclass.h"

VirtualClasses::VirtualClasses():
   mClasses()
{
}

VirtualClass& VirtualClasses::operator[](int index)
{
   return *mClasses[index];
}

// - Query

bool VirtualClasses::contains(const VirtualClass& klass) const
{
   for ( std::size_t index = 0; index < mClasses.size(); ++index )
   {
      if ( mClasses[index]->getName() == klass.getName() )
      {
         return true;
      }
   }
   return false;
}

int VirtualClasses::size() const
{
   return mClasses.size();
}

// - Operations

void VirtualClasses::add(VirtualClass& klass)
{
   mClasses.push_back(&klass);
}

void VirtualClasses::add(VirtualClasses& classes)
{
   for ( int index = 0; index < classes.size(); ++index )
   {
      add(classes[index]);
   }
}

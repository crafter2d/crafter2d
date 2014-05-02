
#ifndef PROPERTY_H
#define PROPERTY_H

namespace ui
{
   template <class T>
   class Property
   {
   public:
      Property();
      Property(T& value);

    // get/set
      T&   value();
      void setValue(T& value);

   private:

    // data
      T mValue;
   };
}

#include "property.inl"

#endif // PROPERTY_H

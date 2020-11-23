
#ifndef TYPES_H
#define TYPES_H

#include <vector>

#include "type.h"

namespace CIL
{
   class Types
   {
   public:
      using TypeList = std::vector<Type>;
      using iterator = TypeList::iterator;
      using const_iterator = TypeList::const_iterator;

      Types();
      Types(const Types& that);
      ~Types();

      const Types& operator=(const Types& that);
      const Type& operator[](int index) const;

      // query
      int size() const;
      bool equals(const Types& that) const;
      bool compareSignature(bool isstatic, const Types& that) const;

     // iterators
      iterator begin() { return mTypes.begin(); }
      iterator end() { return mTypes.end(); }

      const_iterator begin() const { return mTypes.begin(); }
      const_iterator end() const { return mTypes.end(); }


      // operations
      void add(const Type& type);
      void add(Type&& type);
      void clear();

   private:
      

      // data
      TypeList mTypes;
   };
}

#endif // TYPES_H

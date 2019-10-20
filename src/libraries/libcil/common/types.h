
#ifndef TYPES_H
#define TYPES_H

#include <vector>

#include "type.h"

namespace yasc
{
   class Types
   {
   public:
      Types();
      Types(const Types& that);
      ~Types();

      const Types& operator=(const Types& that);
      const Type& operator[](int index) const;

      // query
      int size() const;
      bool equals(const Types& that) const;
      bool compareSignature(bool isstatic, const Types& that) const;

      // operations
      void add(const Type& type);
      void add(Type&& type);
      void clear();

   private:
      typedef std::vector<Type> TypeList;

      // data
      TypeList mTypes;
   };
}

#endif // TYPES_H


#ifndef TYPES_H
#define TYPES_H

#include <vector>

namespace yasc
{
   class Type;

   class Types
   {
   public:
      Types();
      ~Types();

      const Types& operator=(const Types& that);
      const Type& operator[](int index) const;

      // query
      int size() const;
      bool equals(const Types& that) const;
      bool compareSignature(bool isstatic, const Types& that) const;

      // operations
      void add(Type* ptype);
      void clear();

   private:
      typedef std::vector<Type*> TypeList;

      // data
      TypeList mTypes;
   };
}

#endif // TYPES_H

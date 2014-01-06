
#ifndef UUID_H
#define UUID_H

#include <stdint.h>

#include "core/core_base.h"

class DataStream;

namespace c2d
{
   class CORE_API Uuid
   {
   public:
      Uuid();
      Uuid(const Uuid& that);
      Uuid(uint32_t data1, uint16_t data2, uint16_t data3, uint16_t data4, uint64_t data5);

    // operators
      bool operator<(const Uuid& that) const;
      bool operator==(const Uuid& that) const;

    // streaming
      void read(DataStream& stream);
      void write(DataStream& stream) const;

   private:
    // operations
      uint64_t sum() const;

    // data
      uint32_t mData1;
      uint16_t mData2;
      uint16_t mData3;
      uint16_t mData4;
      uint64_t mData5;
   };
}

#endif // UUID_H


#ifndef UUID_H
#define UUID_H

#include <stdint.h>

#include "core/core_base.h"

class DataStream;

class CORE_API Uuid
{
public:
   Uuid();
   Uuid(const Uuid& that);
   Uuid(uint64_t data1, uint16_t data2, uint16_t data3, uint8_t data4[8]);

 // operators
   bool operator==(const Uuid& that) const;

 // streaming
   void read(DataStream& stream);
   void write(DataStream& stream);

private:

 // data
   uint64_t mData1;
   uint16_t mData2;
   uint16_t mData3;
   uint8_t  mData4[8];
};

#endif // UUID_H

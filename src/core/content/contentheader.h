
#ifndef CONTENT_HEADER_H
#define CONTENT_HEADER_H

#include "core/string/string.h"

class DataStream;

class ContentHeader
{
public:
   ContentHeader();

 // get/set
   const String& getName() const;
   void          setName(const String& name);

   const String& getModuleName() const;
   void          setModuleName(const String& name);

 // read/write
   bool read(DataStream& file);
   bool write(DataStream& file);

private:

 // data
   String mModule;
   String mName;
};

#endif // CONTENT_HEADER_H

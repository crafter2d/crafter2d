
#ifndef CONTENT_H
#define CONTENT_H

#include "core/string/string.h"
#include "core/core_base.h"

class CORE_API IContent
{
public:
            IContent();
   virtual ~IContent();

 // get/set
   const String& getFilename() const;
   void          setFilename(const String& name);

private:

 // data
   String mFilename;
};

#endif // CONTENT_H

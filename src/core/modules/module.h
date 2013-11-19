
#ifndef MODULE_H
#define MODULE_H

#include "core/core_base.h"

class CORE_API Module
{
public:
   enum Type { eContent,   // content loader/writer
               eInvalid,
             };

   virtual ~Module();

 // query
   Type getType() const;
   void setType(Type type);

protected:
 // constructors
   explicit Module(Type type);

private:

 // data
   Type mType;
};

#endif // MODULE_H

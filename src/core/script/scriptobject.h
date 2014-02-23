
#ifndef __ScriptObject_h__
#define __ScriptObject_h__

#include "core/string/char.h"
#include "core/system/variant.h"

class String;

namespace c2d
{
   class ScriptObject
   {
   public:

      template<class T> T& get();
      template<class T> T* use();

    // arguments
      virtual void arg(int index, bool arg) = 0;
      virtual void arg(int index, int arg) = 0;
      virtual void arg(int index, float arg) = 0;
      virtual void arg(int index, UChar arg) = 0;
      virtual void arg(int index, const String& arg) = 0;
      virtual void arg(int index, void* pinstance) = 0;
      virtual void arg(int index, const String& classname, void* pinstance) = 0;

    // members
      virtual void setMember(int index, const Variant& value) = 0;

    // instance
      virtual void* getInstance() = 0;
      virtual void* useInstance() = 0;
      virtual void  setInstance(void* pinstance, bool owned = true) = 0;

    // operations
      virtual void prepareCall(int args) = 0;
      virtual Variant call(const String& function) = 0;
      virtual Variant call(const String& function, int argc, Variant* args) = 0;
   };

   template<class T> T& ScriptObject::get()
   {
      return *static_cast<T*>(getInstance());
   }

   template<class T> T* ScriptObject::use()
   {
      return static_cast<T*>(useInstance());
   }
}

#endif // __ScriptObject_h__

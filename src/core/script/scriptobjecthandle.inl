
#include "core/defines.h"

namespace c2d
{
   inline ScriptObjectHandle::ScriptObjectHandle():
      mpObject(nullptr),
      mOwner(false)
   {
   }

   inline ScriptObjectHandle::ScriptObjectHandle(ScriptObject* pobject):
      mpObject(pobject),
      mOwner(true)
   {
   }

   inline ScriptObjectHandle::ScriptObjectHandle(const ScriptObjectHandle& that):
      mpObject(that.mpObject),
      mOwner(that.mOwner)
   {
      that.mOwner = false;
   }

   inline ScriptObjectHandle& ScriptObjectHandle::operator=(const ScriptObjectHandle& that)
   {
      mpObject = that.mpObject;
      mOwner = that.mOwner;

      that.mOwner = false;
      return *this;
   }

   inline ScriptObject* ScriptObjectHandle::operator->()
   {
      return mpObject;
   }

   inline ScriptObject& ScriptObjectHandle::operator*()
   {
      return *mpObject;
   }

   inline ScriptObject* ScriptObjectHandle::use()
   {
      mOwner = false;
      return mpObject;
   }
}

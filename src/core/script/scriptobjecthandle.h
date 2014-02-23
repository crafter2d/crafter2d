
#ifndef SCRIPT_OBJECT_HANDLE_H
#define SCRIPT_OBJECT_HANDLE_H

#include "core/core_base.h"

namespace c2d
{
   class ScriptObject;

   class CORE_API ScriptObjectHandle
   {
   public:
      ScriptObjectHandle();
      ScriptObjectHandle(ScriptObject* pobject);
      ScriptObjectHandle(const ScriptObjectHandle& that);
      ~ScriptObjectHandle();

      ScriptObjectHandle& operator=(const ScriptObjectHandle& that);

      ScriptObject* operator->();
      ScriptObject& operator*();

      ScriptObject* use();

   private:

    // data
      ScriptObject* mpObject;
      mutable bool  mOwner;
   };
}

#include "scriptobjecthandle.inl"

#endif // SCRIPT_OBJECT_HANDLE_H

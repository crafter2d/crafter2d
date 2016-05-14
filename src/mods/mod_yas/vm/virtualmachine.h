/***************************************************************************
 *   Copyright (C) 2006 by Jeroen Broekhuizen                              *
 *   jengine.sse@live.nl                                                   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU Library General Public License as       *
 *   published by the Free Software Foundation; either version 2 of the    *
 *   License, or (at your option) any later version.                       *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU Library General Public     *
 *   License along with this program; if not, write to the                 *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#ifndef VM_H_
#define VM_H_

#include <deque>
#include <unordered_map>
#include <stack>

#include "mod_yas/script_base.h"

#include "core/defines.h"

#include "mod_yas/gc/garbagecollector.h"

#include "virtualfunctiontableentry.h"
#include "virtualcontext.h"
#include "virtualstack.h"
#include "vminterface.h"

namespace ByteCode
{
   class CPU;
}

class ClassRegistry;
class String;
class VirtualArrayException;
class VirtualFunctionTableEntry;
class VirtualException;
class VirtualObject;
class VirtualCall;
class VirtualValue;

class VirtualMachine
{
public:
   explicit VirtualMachine(VirtualContext& context);
   ~VirtualMachine();

 // get/set
   VirtualContext& getContext();

 // initialization
   void initialize();
   
 // loading
   bool loadClass(const String& classname);
   void mergeClassRegistry(const ClassRegistry& registry);

 // execution
   bool         execute(const String& classname, const String& function);
   VirtualValue execute(VirtualObject& object, const String& function, int argc = 0, VirtualValue* pargs = NULL);

 // exception handling
   String buildCallStack() const;
   void displayException(VirtualException& exception);

 // object instantation
   VirtualObject*    instantiate(const String& classname, int constructor = -1);
   VirtualObject&    instantiateNative(const String& classname, void* pobject, bool owned = true);
   VirtualArray*     instantiateArray();
   void              release(VirtualObject& object);

 // garbage collection
   void addRootObject(VirtualObject& object);
   void mark();

 // observing
   VirtualObject*    lookupNative(void* pobject);
   void              registerNative(VirtualObject& object);
   void              unregisterNative(VirtualObject& object);

private:
   friend class GarbageCollector;
   friend class VirtualContext;
   
   typedef std::vector<VirtualObject*> Objects;
   typedef std::unordered_map<void*, VirtualObject*> NativeObjectMap;

   enum State { eInit, eDestruct, eRunning, eFinalizing, eReturn };
   
 // exception
   VirtualObject& instantiateArrayException(const VirtualArrayException& e);
   void           throwException(const String& exceptionname, const String& reason);
   bool           handleException(VirtualException* pexception);

 // class loading
   VirtualClass* doLoadClass(const String& classname);
   void          classLoaded(VirtualClass& klass);
   void          createClassObject(VirtualClass& klass);
   void          registerClass(VirtualClass& aclass);

   VirtualContext&               mContext;
   Objects                       mRootObjects;
   GarbageCollector              mGC;
   NativeObjectMap               mNativeObjects;
   State                         mState;
   ByteCode::CPU*                mpCPU;
};

#endif // VM_H_

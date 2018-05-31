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
#include "virtualmachine.h"

// When testing with Visual Leak Detecter, uncomment the next line
//#include <vld.h>
#include <iostream>

#include "core/script/iscriptable.h"
#include "core/defines.h"

#include "mod_yas/bytecode/program.h"

#include "stackcpu/stackcpu.h"

#include "virtualarray.h"
#include "virtualarrayexception.h"
#include "virtualfunctionnotfoundexception.h"
#include "virtualcontext.h"
#include "virtualobject.h"
#include "virtualclass.h"
#include "virtualexception.h"
#include "virtualfunction.h"
#include "virtualfunctiontable.h"
#include "virtualfunctiontableentry.h"

VirtualMachine::VirtualMachine(VirtualContext& context):
   mContext(context),
   mRootObjects(),
   mGC(),
   mNativeObjects(),
   mState(eInit),
   mpCPU(nullptr)
{
   mpCPU = new StackCPU(*this);

   context.mpVM = this;
}

VirtualMachine::~VirtualMachine()
{
   // set destruct state, native object notifies vm when it is destructed
   // resulting in double delete.
   mState = eDestruct;
   mNativeObjects.clear();
}

// - Get/set

VirtualContext& VirtualMachine::getContext()
{
   return mContext;
}

// - Initialization

void VirtualMachine::initialize()
{
   ClassRegistry registry;
   VMInterface::registerCommonFunctions(registry);
   mergeClassRegistry(registry);

   // initialize the CPU
   mpCPU->initialize(mContext);

   // load internal classes and the ClassLoader
   loadClass(UTEXT("system.InternalString"));
   loadClass(UTEXT("system.InternalArray"));
   loadClass(UTEXT("system.ClassLoader"));

   mState = eRunning;
   
   // register the loaded classes with the ClassLoader instance
   std::vector<VirtualClass*> classes = mContext.mClassTable.asArray();
   for ( std::size_t index = 0; index < classes.size(); index++ )
   {
      VirtualClass* pclass = classes[index];
      createClassObject(*pclass);
      registerClass(*pclass);
   }
}

// - Loading

bool VirtualMachine::loadClass(const String& classname)
{
   return doLoadClass(classname) != nullptr;
}

void VirtualMachine::mergeClassRegistry(const ClassRegistry& registry)
{
   mContext.mNativeRegistry.add(registry);
}

// - Stack access

void VirtualMachine::addRootObject(VirtualObject& object)
{
   mRootObjects.push_back(&object);
}

// - Execution

bool VirtualMachine::execute(const String& classname, const String& function)
{
   const VirtualClass* pclass = doLoadClass(classname);
   if ( pclass == nullptr )
      return false;

   const VirtualFunctionTableEntry* pentry = pclass->getVirtualFunctionTable().findByName(function);
   if ( pentry == nullptr )
      return false;

   mpCPU->executeStatic(mContext, *pclass, *pentry);

   return true;
}

VirtualValue VirtualMachine::execute(VirtualObject& object, const String& function, int argc, VirtualValue* pargs)
{
   const VirtualClass& vclass = object.getClass();
   const VirtualFunctionTableEntry* pentry = vclass.getVirtualFunctionTable().findByName(function);
   if ( pentry == nullptr )
   {
      throw new VirtualFunctionNotFoundException(object.getClass().getName(), function);
   }

   return mpCPU->execute(mContext, object, *pentry, argc, pargs);
}

void VirtualMachine::mark()
{
   mpCPU->mark();

   mContext.mClassTable.mark();  // marks all statics
   for ( std::size_t index = 0; index < mRootObjects.size(); index++ )
   {
      VirtualObject* pobject = mRootObjects[index];
      pobject->mark();
   }
}

// - Exception

String VirtualMachine::buildCallStack() const
{
   return mpCPU->buildCallStack();
}

// - Object creation

/// /fn VirtualMachine::instantiate(const String& classname, int constructor)
/// /brief Objects returned by this function can never be owned by the caller. The
/// objects are managed by the garbage collector. Deleting it, probably results in
/// and access violation when the garbage collector runs.
VirtualObject* VirtualMachine::instantiate(const String& classname, int constructor)
{
   VirtualObject* presult = nullptr;
   VirtualClass* pclass = doLoadClass(classname);
   if ( pclass != nullptr )
   {
      presult = &mpCPU->instantiate(mContext, *pclass, constructor);
   }
   return presult;
}

VirtualObject& VirtualMachine::instantiateNative(const String& classname, void* pobject, bool owned)
{
   ASSERT_PTR(pobject);

   VirtualObject* presult = nullptr;
   NativeObjectMap::iterator it = mNativeObjects.find(pobject);
   if ( it != mNativeObjects.end() )
   {
      presult = it->second;
   }
   else
   {
      presult = instantiate(classname);
      if ( presult == nullptr )
      {
         throw std::runtime_error("Could not instantiate object!");
      }

      mNativeObjects.insert(std::pair<void*, VirtualObject*>(pobject, presult));
   }
   
   presult->setNativeObject(pobject);
   presult->setOwner(owned);
   return *presult;
}

VirtualObject& VirtualMachine::instantiateArrayException(const VirtualArrayException& e)
{
   VirtualObject* presult = nullptr;

   switch ( e.getKind() )
   {
      case VirtualArrayException::eOutOfBounds: 
         {
            presult = instantiate(UTEXT("system.ArrayIndexOutOfBoundsException"), -1);
            break;
         }
      default:
         UNREACHABLE("Invalid enum value");
   }
   
   ASSERT_PTR(presult);
   return *presult;
}

VirtualObject* VirtualMachine::lookupNative(void* pobject)
{
   NativeObjectMap::iterator it = mNativeObjects.find(pobject);
   if ( it != mNativeObjects.end() )
   {
      ASSERT(it->second->getNativeObjectPtr() == pobject);
      return it->second;
   }
   return nullptr;
}

VirtualArray* VirtualMachine::instantiateArray()
{
   VirtualArray* parray = new VirtualArray();
   mGC.collect(parray);
   return parray;
}

void VirtualMachine::release(VirtualObject& object)
{
   mpCPU->release(object);
}

// - Native interface

void VirtualMachine::registerNative(VirtualObject& object)
{
   ASSERT(object.hasNativeObject());
   std::pair<NativeObjectMap::iterator,bool> ret = mNativeObjects.insert(std::pair<void*, VirtualObject*>(object.getNativeObjectPtr(), &object));
   ASSERT(ret.second);
}

void VirtualMachine::unregisterNative(VirtualObject& object)
{
   // to be decided what to do with this.. called from the GC??

   ASSERT(object.hasNativeObject());

   // remove the object from the map
   NativeObjectMap::iterator it = mNativeObjects.find(object.getNativeObjectPtr());
   ASSERT(it != mNativeObjects.end());
   mNativeObjects.erase(it);

   if ( object.isOwner() )
   {
      static String sFinal = UTEXT("finalize");
      const VirtualFunctionTableEntry* pentry = object.getClass().getVirtualFunctionTable().findByName(sFinal);
      if ( pentry != nullptr )
      {
         mpCPU->execute(mContext, object, *pentry, 0, nullptr);
      }
   }

   object.setNativeObject(nullptr);
}

// - Class loading

VirtualClass* VirtualMachine::doLoadClass(const String& classname)
{
   try
   {
      return &mContext.resolveClass(classname);
   }
   catch ( ... )
   {
      return nullptr;
   }
}

void VirtualMachine::classLoaded(VirtualClass& klass)
{
   if ( mState > eDestruct )
   {
      createClassObject(klass);
      registerClass(klass);
   }

   // execute the static initialization body
   VirtualFunctionTableEntry& entry = klass.getVirtualFunctionTable()[0];
   mpCPU->executeStatic(mContext, klass, entry);
}

void VirtualMachine::createClassObject(VirtualClass& klass)
{
   static const String sClass = UTEXT("system.Class");
   static const String sFunction = UTEXT("system.Function");

   const VirtualFunctionTable& table = klass.getVirtualFunctionTable();
   VirtualArray* pfuncarray = new VirtualArray();
   pfuncarray->addLevel(table.size());

   const VirtualClass& funcclass = mContext.resolveClass(sFunction);

   for ( int index = 0; index < table.size(); index++ )
   {
      const VirtualFunctionTableEntry& function = table[index];

      VirtualArray* pannoarray = new VirtualArray();
      const StringList& annotations = function.mpFunction->getAnnotations();
      pannoarray->addLevel(annotations.size());
      if ( annotations.size() > 0 )
      {
         int idx = 0;
         for ( auto& value : annotations )
         {
            VirtualString& vname = mContext.mStringCache.lookup(value);
            (*pannoarray)[idx++] = VirtualValue(vname);
         }
      }

      VirtualObject* funcobject = new VirtualObject();
      funcobject->initialize(2);
      funcobject->setClass(funcclass);
      funcobject->setMember(0, VirtualValue(mContext.mProgram.getStringCache().lookup(function.getName())));
      funcobject->setMember(1, VirtualValue(*pannoarray));

      (*pfuncarray)[index] = VirtualValue(*funcobject); // <-- hack!
   }

   VirtualObject* classobject = new VirtualObject();
   classobject->initialize(2);
   classobject->setClass(mContext.resolveClass(sClass));
   classobject->setMember(0, VirtualValue(mContext.mProgram.getStringCache().lookup(klass.getName())));
   classobject->setMember(1, VirtualValue(*pfuncarray));

   klass.setClassObject(classobject);
}

void VirtualMachine::registerClass(VirtualClass& klass)
{
   static const String sClassLoader = UTEXT("system.ClassLoader");

   // notify the ClassLoader to add this class
   const VirtualClass& classloader = mContext.resolveClass(sClassLoader);
   const VirtualFunctionTableEntry& entry = classloader.getVirtualFunctionTable()[4];
   const VirtualValue& classloaderobject = classloader.getStatic(0);
   ASSERT(classloaderobject.isObject());

   VirtualValue arg(klass.getClassObject());
   mpCPU->execute(mContext, classloaderobject.asObject(), entry, 1, &arg);
}

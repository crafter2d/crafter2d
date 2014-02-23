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

#include "mod_yas/bytecode/generator.h"
#include "mod_yas/bytecode/program.h"

#include "stackcpu/stackcpu.h"

#include "virtualarray.h"
#include "virtualarrayexception.h"
#include "virtualfunctionnotfoundexception.h"
#include "virtualcontext.h"
#include "virtualobject.h"
#include "virtualclass.h"
#include "virtualexception.h"
#include "virtualfunctiontable.h"
#include "virtualfunctiontableentry.h"

VirtualMachine::VirtualMachine(VirtualContext& context):
   mContext(context),
   mRootObjects(),
   mGC(),
   mNativeObjects(),
   mState(eInit),
   mpCPU(NULL),
   mpGenerator(NULL)
{
   mpCPU = new StackCPU(*this);
   context.mProgram.setByteCodeGenerator(mpCPU->createIRGenerator());
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

   mpGenerator = new ByteCode::Generator();
   if ( mpGenerator == NULL )
   {
      throw new std::exception("Out of memory");
   }

   // preload some common classes
   loadClass(UTEXT("system.Object"));
   loadClass(UTEXT("system.InternalArray"));
   loadClass(UTEXT("system.InternalString"));
   loadClass(UTEXT("system.AssertionError"));
   loadClass(UTEXT("system.NullPointerException"));

   // resolve the internal classes
   mpCPU->initialize(mContext);

   // pre-compile utility classes
   loadClass(UTEXT("system.ClassLoader"));
   loadClass(UTEXT("system.System"));

   mState = eRunning;
   
   // register the loaded classes with the ClassLoader instance
   std::vector<VirtualClass*> classes = mContext.mClassTable.asArray();
   for ( std::size_t index = 0; index < classes.size(); index++ )
   {
      VirtualClass* pclass = classes[index];
      createClass(*pclass);
   }
}

// - Loading

bool VirtualMachine::loadClass(const String& classname)
{
   return doLoadClass(classname) != NULL;
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
   if ( pclass == NULL )
      return false;

   const VirtualFunctionTableEntry* pentry = pclass->getVirtualFunctionTable().findByName(function);
   if ( pentry == NULL )
      return false;

   mpCPU->executeStatic(mContext, *pclass, *pentry);

   return true;
}

VirtualValue VirtualMachine::execute(VirtualObject& object, const String& function, int argc, VirtualValue* pargs)
{
   const VirtualClass& vclass = object.getClass();
   const VirtualFunctionTableEntry* pentry = vclass.getVirtualFunctionTable().findByName(function);
   if ( pentry == NULL )
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
   VirtualObject* presult = NULL;
   VirtualClass* pclass = doLoadClass(classname);
   if ( pclass != NULL )
   {
      presult = &mpCPU->instantiate(mContext, *pclass, constructor);
   }
   return presult;
}

VirtualObject* VirtualMachine::instantiateNative(const String& classname, void* pobject, bool owned)
{
   ASSERT_PTR(pobject);

   VirtualObject* presult = NULL;
   NativeObjectMap::iterator it = mNativeObjects.find(pobject);
   if ( it != mNativeObjects.end() )
   {
      presult = it->second;
   }
   else
   {
      presult = instantiate(classname);
      if ( presult == NULL )
      {
         return NULL;
      }

      mNativeObjects.insert(std::pair<void*, VirtualObject*>(pobject, presult));
   }
   
   presult->setNativeObject(pobject);
   presult->setOwner(owned);
   return presult;
}

VirtualObject& VirtualMachine::instantiateArrayException(const VirtualArrayException& e)
{
   VirtualObject* presult;

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
   return NULL;
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

void VirtualMachine::registerNative(VirtualObject& object, void* pnative)
{
   ASSERT(!object.hasNativeObject());
   object.setNativeObject(pnative);
   object.setOwner(true);

   std::pair<NativeObjectMap::iterator,bool> ret = mNativeObjects.insert(std::pair<void*, VirtualObject*>(pnative, &object));
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

   object.setNativeObject(NULL);

   if ( object.isOwner() )
   {
      static String sFinal = UTEXT("finalize");
      const VirtualFunctionTableEntry* pentry = object.getClass().getVirtualFunctionTable().findByName(sFinal);
      if ( pentry != NULL )
      {
         mpCPU->execute(mContext, object, *pentry, 0, NULL);
      }
   }
}

// - Callbacks

VirtualClass* VirtualMachine::doLoadClass(const String& classname)
{
   VirtualClass* pclass = mContext.mClassTable.find(classname);
   if ( pclass == NULL )
   {
      // TODO : load the class from CIL
      pclass = &mpGenerator->generate(mContext, classname);
   }
   return pclass;
}

void VirtualMachine::classLoaded(VirtualClass* pclass)
{
   mContext.mClassTable.insert(pclass);

   createClass(*pclass);

   // execute the static initialization body
   VirtualFunctionTableEntry& entry = pclass->getVirtualFunctionTable()[0];
   mpCPU->executeStatic(mContext, *pclass, entry);
}

void VirtualMachine::createClass(const VirtualClass& aclass)
{
   if ( mState > eDestruct )
   {
      static const String sClass = UTEXT("system.Class");
      static const String sFunction = UTEXT("system.Function");
      static const String sClassLoader = UTEXT("system.ClassLoader");

      VirtualObject& object = aclass.getClassObject();

      // resolve the virtual classes
      object.setClass(mContext.mClassTable.resolve(sClass));

      VirtualArray& funcarray = object.getMember(1).asArray();
      const VirtualClass& funcclass = mContext.mClassTable.resolve(sFunction);
      for ( int index = 0; index < funcarray.size(); index++ )
      {
         VirtualObject& func = funcarray[index].asObject();
         func.setClass(funcclass);
      }

      // notify the ClassLoader to add this class
      const VirtualClass& classloader = mContext.mClassTable.resolve(sClassLoader);
      const VirtualFunctionTableEntry& entry = classloader.getVirtualFunctionTable()[4];
      const VirtualValue& classloaderobject = classloader.getStatic(0);
      ASSERT(classloaderobject.isObject());

      VirtualValue arg(object);
      mpCPU->execute(mContext, classloaderobject.asObject(), entry, 1, &arg);
   }
}

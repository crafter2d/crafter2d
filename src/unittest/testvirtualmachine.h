
#ifndef TEST_VIRTUALMACHINE_H
#define TEST_VIRTUALMACHINE_H

#include <unittest++/UnitTest++.h>

#include "core/vfs/filesystem.h"
#include "core/modules/modulemanager.h"
#include "core/modules/modulecollection.h"
#include "core/modules/scriptmodule.h"
#include "core/script/scriptcall.h"
#include "core/script/scriptobjecthandle.h"
#include "core/script/scriptobject.h"
#include "core/script/scriptmanager.h"
#include "core/script/scriptregistrator.h"

using namespace c2d;

class NativeClass
{
public:
   int    getIntValue() { return 5; }
   String getStringValue() { return UTEXT("test"); }

   int add(int a, int b) { return a + b; }
   int sub(int a, int b) { return a - b; }
   int mul(int a, int b) { return a * b; }
};

void NativeClass_init(ScriptCall& accessor)
{
   NativeClass* pnative = new NativeClass();
   ScriptObjectHandle thisobject = accessor.getObject(0);
   thisobject->setInstance(pnative);
}

void NativeClass_getIntValue(ScriptCall& accessor)
{
   NativeClass* pnative = accessor.getObject(0)->get<NativeClass*>();

   accessor.setResult(pnative->getIntValue());
}

void NativeClass_getStringValue(ScriptCall& accessor)
{
   NativeClass* pnative = accessor.getObject(0)->get<NativeClass*>();

   accessor.setResult(pnative->getStringValue());
}

void NativeClass_add(ScriptCall& accessor)
{
   NativeClass* pnative = accessor.getObject(0)->get<NativeClass*>();

   int a = accessor.getInt(1);
   int b = accessor.getInt(2);

   accessor.setResult(pnative->add(a, b));
}

void NativeClass_sub(ScriptCall& accessor)
{
   NativeClass* pnative = accessor.getObject(0)->get<NativeClass*>();

   int a = accessor.getInt(1);
   int b = accessor.getInt(2);

   accessor.setResult(pnative->sub(a, b));
}

void NativeClass_mul(ScriptCall& accessor)
{
   NativeClass* pnative = accessor.getObject(0)->get<NativeClass*>();

   int a = accessor.getInt(1);
   int b = accessor.getInt(2);

   accessor.setResult(pnative->mul(a, b));
}

SUITE(TestVirtualMachine)
{
   TEST(testRun)
   {      
      ModuleManager modulemgr;
      CHECK(modulemgr.initialize());
      c2d::Module* pmodule = modulemgr.lookup(c2d::UUID_ScriptModule);
      if ( pmodule == nullptr )
      {
         // no need to continue testing as it seems there is no script module available
         return;
      }

      ScriptModule* pmod = static_cast<c2d::ScriptModule*>(pmodule);
      ScriptManager& scriptmanager = pmod->getManager();

      std::unique_ptr<ScriptRegistrator> pregistrator(scriptmanager.getRegistrator());

      pregistrator->addClass(UTEXT("NativeClass"));
      pregistrator->addFunction(UTEXT("NativeClass()"), NativeClass_init);
      pregistrator->addFunction(UTEXT("getIntValue()"), NativeClass_getIntValue);
      pregistrator->addFunction(UTEXT("getStringValue()"), NativeClass_getStringValue);
      pregistrator->addFunction(UTEXT("add(int, int)"), NativeClass_add);
      pregistrator->addFunction(UTEXT("sub(int, int)"), NativeClass_sub);
      pregistrator->addFunction(UTEXT("mul(int, int)"), NativeClass_mul);

      pregistrator->registerCallbacks();

      FileSystem& fs = FileSystem::getInstance();
      fs.removeAll();
      fs.addPath(UTEXT("data.zip/scripts"));
      fs.addPath(UTEXT("../src/unittest/test.zip/test"));

      std::unique_ptr<ScriptObject> script(scriptmanager.load(UTEXT("Test")));
      scriptmanager.addRootObject(*script);

      CHECK(script.get() != nullptr);
      Variant result = script->call(UTEXT("run"));
      CHECK(result.isBool());
      CHECK(result.asBool());
   }

};

#endif // TEST_VIRTUALMACHINE_H

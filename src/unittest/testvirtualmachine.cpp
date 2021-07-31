
#include <gtest/gtest.h>

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

TEST(VirtualMachine, Run)
{
   ModuleManager modulemgr;
   EXPECT_TRUE(modulemgr.initialize());
   c2d::Module* pmodule = modulemgr.lookup(c2d::UUID_ScriptModule);
   ASSERT_NE(pmodule, nullptr);

   auto& scriptmod = static_cast<c2d::ScriptModule&>(*pmodule);
   auto& scriptmanager = scriptmod.getManager();

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
   EXPECT_NE(script.get(), nullptr);
   Variant result = script->call(UTEXT("run"));
   EXPECT_TRUE(result.isBool());
   EXPECT_TRUE(result.asBool());
}

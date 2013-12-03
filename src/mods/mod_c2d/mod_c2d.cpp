// mod_effect.cpp : Defines the exported functions for the DLL application.
//

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <windows.h>

#include "core/content/contentmodule.h"
#include "core/modules/modulecollection.h"
#include "core/graphics/effect.h"
#include "core/system/uuid.h"

#include "effect/effectreader.h"
#include "effect/effectwriter.h"

#include "entity/entityreader.h"
#include "entity/entitywriter.h"

#include "texture/texturereader.h"
#include "texture/texturewriter.h"

#ifdef WIN32
#ifdef MOD_EXPORTS
#define MOD_API __declspec(dllexport)
#else
#define MOD_API __declspec(dllimport)
#endif
#else
#define MOD_API
#endif

// {569B9122-695E-46D5-A1F6-EC808010572D}
uint8_t effectval[] = { 0xa1, 0xf6, 0xec, 0x80, 0x80, 0x10, 0x57, 0x2d };
static const Uuid EffectGUID(0x569b9122, 0x695e, 0x46d5, effectval);

// {89E326B3-1606-4137-BA81-8EBD5A158EEC}
uint8_t entityval[] = { 0xba, 0x81, 0x8e, 0xbd, 0x5a, 0x15, 0x8e, 0xec };
static const Uuid EntityGUID(0x89e326b3, 0x1606, 0x4137, entityval);

// {EEDD2CF4-ABA2-4831-9966-C14FE9F1CDFC}
uint8_t textureval[] = { 0x99, 0x66, 0xc1, 0x4f, 0xe9, 0xf1, 0xcd, 0xfc };
static const Uuid TextureGUID(0xeedd2cf4, 0xaba2, 0x4831, textureval);


extern "C" MOD_API ModuleCollection* cdecl getModuleCollection()
{
   ModuleCollection* pmodules = new ModuleCollection();

   ContentModule* peffectmodule = new ContentModule();
   peffectmodule->setSupportedFiles(UTEXT("fx"));
   peffectmodule->setUuid(EffectGUID);
   peffectmodule->setReader(new EffectReader());
   peffectmodule->setWriter(new EffectWriter());
   pmodules->add(peffectmodule);

   ContentModule* pentitymod = new ContentModule();
   pentitymod->setSupportedFiles(UTEXT("xml"));
   pentitymod->setUuid(EntityGUID);
   pentitymod->setReader(new EntityReader());
   pentitymod->setWriter(new EntityWriter());
   pmodules->add(pentitymod);

   ContentModule* ptexturemod = new ContentModule();
   ptexturemod->setSupportedFiles(UTEXT("dds"));
   ptexturemod->setUuid(TextureGUID);
   ptexturemod->setReader(new TextureReader());
   ptexturemod->setWriter(new TextureWriter());
   pmodules->add(ptexturemod);

   return pmodules;
}

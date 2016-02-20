// mod_effect.cpp : Defines the exported functions for the DLL application.
//

#ifdef WIN32
#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <windows.h>
#endif

#include "core/modules/contentmodule.h"
#include "core/modules/modulecollection.h"
#include "core/graphics/effect.h"
#include "core/system/uuid.h"

#include "effect/effectreader.h"
#include "effect/effectwriter.h"

#include "entity/entityreader.h"
#include "entity/entitywriter.h"

#include "sound/soundreader.h"
#include "sound/soundwriter.h"

#include "texture/texturereader.h"
#include "texture/texturewriter.h"

#include "tileatlas/tileatlasreader.h"
#include "tileatlas/tileatlaswriter.h"

#include "tileset/tilesetreader.h"
#include "tileset/tilesetwriter.h"

#include "world/worldreader.h"
#include "world/worldwriter.h"

#if defined(WIN32)
  #define MOD_DECL cdecl
  #ifdef MOD_EXPORTS
    #define MOD_API __declspec(dllexport)
  #else
    #define MOD_API __declspec(dllimport)
  #endif
#else
  #define MOD_API
  #define MOD_DECL
#endif

using namespace c2d;

// {569B9122-695E-46D5-A1F6-EC808010572D}
static const Uuid EffectUUID(0x569B9122, 0x695E, 0x46D5, 0xA1F6, 0xec808010572d);

// {89E326B3-1606-4137-BA81-8EBD5A158EEC}
static const Uuid EntityUUID(0x89E326B3, 0x1606, 0x4137, 0xBA81, 0x8EBD5A158EEC);

// {C21021BC-B904-4FC5-95B3-D7BC2218C92B}
static const Uuid SoundUUID(0xC21021BC, 0xB904, 0x4FC5, 0x95B3, 0xD7BC2218C92B);

// {EEDD2CF4-ABA2-4831-9966-C14FE9F1CDFC}
static const Uuid TextureUUID(0xEEDD2CF4, 0xABA2, 0x4831, 0x9966, 0xC14FE9F1CDFC);

// {7E8CA75D-5E6C-4543-9079-5DC95C50881C}
static const Uuid TileAtlasUUID(0x7e8ca75d, 0x5E6C, 0x4543, 0x9079, 0x5DC95C50881C);

// {F7150E51-BEC1-42AD-9C18-B5B69F86D05E}
static const Uuid TileSetUUID(0xF7150E51, 0xBEC1, 0x42AD, 0x9C18, 0xB5B69F86D05E);

// {54034D72-41BF-4DC5-A2B8-F4DB28B3397E}
static const Uuid WorldUUID(0x54034D72, 0x41BF, 0x4DC5, 0xA2B8, 0xF4DB28B3397E);

extern "C" MOD_API ModuleCollection* MOD_DECL getModuleCollection()
{
   ModuleCollection* pmodules = new ModuleCollection();

   ContentModule* peffectmodule = new ContentModule(EffectUUID);
   peffectmodule->setSupportedFiles(UTEXT("fx"));
   peffectmodule->setReader(new EffectReader());
   peffectmodule->setWriter(new EffectWriter());
   pmodules->add(peffectmodule);

   ContentModule* pentitymod = new ContentModule(EntityUUID);
   pentitymod->setSupportedFiles(UTEXT("entity"));
   pentitymod->setReader(new EntityReader());
   pentitymod->setWriter(new EntityWriter());
   pmodules->add(pentitymod);

   ContentModule* psoundmod = new ContentModule(SoundUUID);
   psoundmod->setSupportedFiles(UTEXT("ogg"));
   psoundmod->setReader(new SoundReader());
   psoundmod->setWriter(new SoundWriter());
   pmodules->add(psoundmod);

   ContentModule* ptexturemod = new ContentModule(TextureUUID);
   ptexturemod->setSupportedFiles(UTEXT("dds,png"));
   ptexturemod->setReader(new TextureReader());
   ptexturemod->setWriter(new TextureWriter());
   pmodules->add(ptexturemod);

   ContentModule* ptileatlasmod = new ContentModule(TileAtlasUUID);
   ptileatlasmod->setSupportedFiles(UTEXT("tileatlas"));
   ptileatlasmod->setReader(new TileAtlasReader());
   ptileatlasmod->setWriter(new TileAtlasWriter());
   pmodules->add(ptileatlasmod);

   ContentModule* ptilesetmod = new ContentModule(TileSetUUID);
   ptilesetmod->setSupportedFiles(UTEXT("tileset"));
   ptilesetmod->setReader(new TileSetReader());
   ptilesetmod->setWriter(new TileSetWriter());
   pmodules->add(ptilesetmod);

   ContentModule* pworldmod = new ContentModule(WorldUUID);
   pworldmod->setSupportedFiles(UTEXT("world"));
   pworldmod->setReader(new WorldReader());
   pworldmod->setWriter(new WorldWriter());
   pmodules->add(pworldmod);

   return pmodules;
}

#include "abstracttextureloader.h"
#ifndef JENGINE_INLINE
#  include "abstracttextureloader.inl"
#endif

#include <SDL/SDL.h>
#include <memory>

#include "../defines.h"
#include "../autoptr.h"

#include "../vfs/file.h"
#include "../vfs/filesystem.h"

AbstractTextureLoader::AbstractTextureLoader():
   mTextureInfo()
{
}

AbstractTextureLoader::~AbstractTextureLoader()
{
}

bool AbstractTextureLoader::load(const std::string& filename)
{
   AutoPtr<File> pfile = FileSystem::getInstance().open(filename);

   bool success = virLoad(*pfile, mTextureInfo);

   return success;
}

bool AbstractTextureLoader::virLoad(File& file, TextureInfo& info)
{
   PURE_VIRTUAL
   return false;
}
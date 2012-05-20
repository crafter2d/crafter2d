
#include "generator.h"

#include "core/defines.h"

Generator::Generator()
{
}

Generator::~Generator()
{
}

// get/set
const FileSystem& Generator::getFileSystem() const
{
   ASSERT_PTR(mpFileSystem);
   return *mpFileSystem;
}

void Generator::setFileSystem(FileSystem& filesystem)
{
   mpFileSystem = &filesystem;
}

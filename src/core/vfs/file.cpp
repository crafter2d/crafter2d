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
#include "file.h"

#include "core/defines.h"
#include "core/string/string.h"

#include "buffer.h"
#include "filesystem.h"

// static 
String File::concat(const String& path, const String& filename)
{
   if ( path.isEmpty() )
      return filename;
   else if ( filename.isEmpty() )
      return path;

   bool has = path[path.length()-1] == '/' || path[path.length()-1] == '\\'
           || filename[0] == '/' || filename[0] == '\\';
   if ( has )
   {
      return path + filename;
   }
   return path + FileSystem::getNativeSeparator() + filename;
}

// static
String File::extractFileName(const String& filepath)
{
   int pos = filepath.lastIndexOf('/');
   if ( pos == -1 )
   {
      pos = filepath.lastIndexOf('\\');
   }

   if ( pos != -1 )
   {
      return filepath.subStr(pos + 1, filepath.length() - pos - 1);
   }

   return filepath;
}

// static
String File::extractPath(const String& filepath)
{
   int pos = filepath.lastIndexOf('/');
   if ( pos == -1 )
   {
      pos = filepath.lastIndexOf('\\');
   }

   if ( pos != -1 )
   {
      return filepath.subStr(0, pos);
   }

   return UTEXT("");
}

// static 
String File::toNativeSeparator(const String& filepath)
{
   UChar sep = FileSystem::getInstance().getSeparator();
   String result = filepath;
   if ( sep == L'/' )
      result.replace(L'\\', sep);
   else
      result.replace(L'/', sep);
   return result;
}

File::File():
   mFilename(),
   mpBuffer(NULL)
{
}

File::~File()
{
}

Buffer& File::getBuffer()
{
   ASSERT_PTR(mpBuffer)
   return *mpBuffer;
}

void File::setBuffer(Buffer* pbuffer)
{
   delete mpBuffer;
   mpBuffer = pbuffer;
}

bool File::open(const String& filename, int modus)
{
   mFilename = filename;
   if ( virOpen(filename, modus) )
   {
      getBuffer().setWritting(!IS_SET(modus, File::ERead));

      return true;
   }

   return false;
}

void File::close()
{
   virClose();
}

// - Get/set

const String& File::getFileName() const
{
   return mFilename;
}

// - Reading & writting

int File::read(void* ptr, int size)
{
   return getBuffer().read(ptr, size);
}

int File::write(const void* ptr, int size)
{
   return getBuffer().write(ptr, size);
}

int File::write(const String& text)
{
   std::string data = text.toUtf8();
   return getBuffer().write((void*)data.c_str(), data.length());
}

char File::getc()
{
   return getBuffer().getchar();
}

char File::peekc()
{
   return getBuffer().peekchar();
}

// Search & positioning

void File::seek(int pos, int mode)
{
   return getBuffer().seek(pos, mode);
}

int File::tell() const
{
   return mpBuffer->tell();
}

bool File::eof() const
{
   return mpBuffer != NULL ? mpBuffer->eof() : true;
}

// - Query

int File::size()
{
   return getBuffer().size();
}

bool File::virOpen(const String& , int)
{
   PURE_VIRTUAL
   return false;
}

void File::virClose()
{
   delete mpBuffer;
   mpBuffer = NULL;
}

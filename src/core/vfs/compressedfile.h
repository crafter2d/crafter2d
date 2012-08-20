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
#ifndef COMPRESSED_FILE_H_
#define COMPRESSED_FILE_H_

#include "core/string/string.h"

#include "file.h"

class CompressedFile : public File
{
public:
   static bool isCompressedFile(const String& file);
   static bool exists(const String& path, const String& file);

   CompressedFile();
   virtual ~CompressedFile();

   virtual bool isValid() const;

protected:
 // operations
   virtual bool  virOpen(const String& filename, int modus);
   virtual void  virClose();

private:
   void decode(const String& filename);

   String mZipFile;
   String mFile;
};

#ifdef JENGINE_INLINE
#  include "compressedfile.inl"
#endif

#endif // COMPRESSED_FILE_H_

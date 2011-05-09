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
#include "stdiofile.h"

#include "core/defines.h"

#include "buffer.h"

// static 
bool StdioFile::exists(const std::string& file)
{
   bool result = false;
   FILE* pfile = fopen(file.c_str(), "r");
   if ( pfile != NULL )
   {
      result = true;
      fclose(pfile);
   }
   return result;
}

StdioFile::StdioFile():
   File(),
   mpFile(NULL)
{
}

StdioFile::~StdioFile()
{
   virClose();
}

bool StdioFile::virOpen(const std::string& filename, int modus)
{
   std::string mode;
   if ( IS_SET(modus, ERead) )
      mode = 'r';
   else
      mode = 'w';

   if ( IS_SET(modus, EBinary) )
      mode += 'b';

   mpFile = fopen(filename.c_str(), mode.c_str());

   setBuffer(Buffer::fromFile(mpFile));

   return true;
}

void StdioFile::virClose()
{
   File::virClose();

   fclose(mpFile);
   mpFile = NULL;
}

bool StdioFile::isValid() const
{
   return mpFile != NULL;
}

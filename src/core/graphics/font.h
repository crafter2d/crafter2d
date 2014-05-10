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
#ifndef FONT_H_
#define FONT_H_

#include <map>

#include "core/string/string.h"
#include "core/core_base.h"

namespace Graphics
{
   class GlyphAtlas;

   class CORE_API Font
   {
   public:
      Font();
      virtual ~Font();

    // get/set
      const String& getFamilyName() const;
      void          setFamilyName(const String& name);

      GlyphAtlas& getGlyphAtlas();
      void        setGlyphAtlas(GlyphAtlas* patlas);

    // query
      virtual int      getBaseLine() const = 0;

    // sizes
      virtual int      getTextWidth(const String& text) const = 0;
      virtual int      getTextHeight(const String& text) const = 0;

    // rendering
      virtual void     render(const String& text) = 0;

   private:

    // data
      GlyphAtlas*    mpGlyphAtlas;
      String         mFamilyName;
   };
};

#endif // FONT_H_

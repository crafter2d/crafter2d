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
#ifndef VERTEXBUFFER_OBJECT_H
#define VERTEXBUFFER_OBJECT_H

#include "GL/gl3w.h"

#include "core/graphics/vertexbuffer.h"

namespace Graphics
{
   /**
   @author Jeroen Broekhuizen
   */
   class OGLVertexBuffer : public VertexBuffer
   {
   public:
                OGLVertexBuffer();
       virtual ~OGLVertexBuffer();

       virtual bool create(const VertexLayout& layout, int length, int usage) override;
	    void release();

       virtual void* lock(RenderContext& context) override;
       virtual void unlock(RenderContext& context) override;

       virtual void update(RenderContext& context, void* pdata, int count) override;

       virtual void enable(RenderContext& context) const;
       virtual void disable(RenderContext& context) const;

   private:

    // data
      GLuint mVAO;
      GLuint mBuffer;
      bool   locked;
   };
};

#endif // OGL_VERTEXBUFFER_H

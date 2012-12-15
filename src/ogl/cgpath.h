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
#ifndef CG_PATH_H_
#define CG_PATH_H_

#include <vector>
#include <Cg/cg.h>

#include "core/graphics/codepath.h"

class String;

namespace Graphics
{
   class CGEngine;

   /*!
   @author Jeroen Broekhuizen
   \brief Code path that uses CG of NVidia to handle the shaders. 

   This code path enables you to use the famous CG shading language of NVidia
   in your games. It does not require additional shader objects, as CG takes
   care of it.

   A requirement of this code path is that at least ARBVP1 vertex programs
   must be supported on the target platform. Currently there is no fallback
   mechanism to another profile (this is also not wanted as in the shaders
   the opengl state variables are used).
    */
   class CGPath : public CodePath
   {
   public:
	   explicit CGPath(CGEngine& engine);
	   virtual ~CGPath();

	   virtual bool     load (const String& vertex, const String& fragment);
	   virtual void     release ();

	   virtual void     enable() const;
	   virtual void     disable() const;
	
      virtual PathType getType() const;
	   virtual int      getUniformLocation (const String& name) const;

      virtual void     setStateUniform(int index);
	   virtual void     setUniform1i (int index, int val);
	   virtual void     setUniform3f (int index, float x, float y, float z);
	   virtual void     setUniform4f (int index, float x, float y, float z, float w);

   private:
      bool  loadVertexProgram(const String& vertexfile);
      bool  loadFragmentProgram(const String& fragmentfile);

      void  CGCompileError(CGcontext context, const String& type, const String& file);

      CGEngine& mCG;
      CGprogram   vp, fp;
      mutable std::vector<CGparameter> _userparams;
   };
};

#ifdef JENGINE_INLINE
#  include "cgpath.inl"
#endif

#endif

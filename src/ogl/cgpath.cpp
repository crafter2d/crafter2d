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
#include "cgpath.h"
#ifndef JENGINE_INLINE
#  include "cgpath.inl"
#endif

#include "core/log/log.h"
#include "core/string/string.h"
#include "core/defines.h"

#include "cgengine.h"
#include "ogldevice.h"

using namespace Graphics;

CGPath::CGPath(CGEngine& cg): 
   CodePath(),
   mCG(cg),
   vp(0),
   fp(0),
   _userparams()
{
}

CGPath::~CGPath()
{
}

bool CGPath::load(VertexInputLayout& layout, const String& vertex, const String& fragment)
{
   return loadVertexProgram(vertex) && loadFragmentProgram(fragment);
}

bool CGPath::loadVertexProgram(const String& vertexfile)
{
   int len = 0;
   const char* pfilename = vertexfile.toUtf8(len);

   vp = cgCreateProgramFromFile(mCG.getContext(), 
                                CG_SOURCE, 
                                pfilename, 
                                mCG.getVertexProfile(), 
                                "main", 
                                NULL);

   delete[] pfilename;

   if (vp == NULL)
   {
      CGCompileError(mCG.getContext(), "vertex", vertexfile);
      return false;
   }

   cgGLLoadProgram (vp);

   return true;
}

bool CGPath::loadFragmentProgram(const String& fragmentfile)
{
   int len = 0;
   const char* pfilename = fragmentfile.toUtf8(len);
   
   fp = cgCreateProgramFromFile(mCG.getContext(), 
                                CG_SOURCE,
                                pfilename, 
                                mCG.getFragmentProfile(), 
                                "main", 
                                NULL);

   delete[] pfilename;

   if ( fp == NULL )
   {
      CGCompileError(mCG.getContext(), "fragment", fragmentfile);
      return false;
   }

   cgGLLoadProgram (fp);

   return true;
}

void CGPath::CGCompileError(CGcontext context, const String& type, const String& file)
{
   Log& log = Log::getInstance();
   const char* list = cgGetLastListing(context);

   log.error("CG Path - Could not load %s program: %s", type.getBuffer(), file.getBuffer());
   if ( list != NULL )
      log.info("%s", list);
}

void CGPath::release()
{
   if ( vp != 0 )
   {
      cgDestroyProgram(vp);
      vp = NULL;
   }

   if ( fp != 0 )
   {
      cgDestroyProgram(fp);
      fp = NULL;
   }
}

void CGPath::enable() const
{
   if ( vp != 0 )
   {
      cgGLEnableProfile( mCG.getVertexProfile() );
      cgGLBindProgram( vp );
   }

   if ( fp != 0 )
   {
      cgGLEnableProfile( mCG.getFragmentProfile() );
      cgGLBindProgram( fp );
   }
}

void CGPath::disable() const
{
   if ( vp != 0 )
      cgGLDisableProfile( mCG.getVertexProfile() );

   if ( fp != 0 )
      cgGLDisableProfile( mCG.getFragmentProfile() );
}

CodePath::PathType CGPath::getType() const
{
   return CodePath::ECG;
}

int CGPath::getUniformLocation(const String& name) const
{
   CGparameter param = 0;
   int len;
   const char* pname = name.toUtf8(len);

   if ( vp != NULL )
      param = cgGetNamedParameter(vp, pname);

   if ( fp != NULL && param == 0 )
      param = cgGetNamedParameter(fp, pname);

   delete[] pname;

   _userparams.push_back(param);
   return _userparams.size() - 1;
}

void CGPath::setUniformStateMatrix(int index) const
{
   CGparameter param = _userparams[index];
   cgGLSetStateMatrixParameter(param, CG_GL_MODELVIEW_PROJECTION_MATRIX, CG_GL_MATRIX_IDENTITY);
}

void CGPath::setUniform1i(int index, int val)
{
   //CGparameter parm = (CGparameter)index;
   CGparameter param = _userparams[index];
   CGtype type = cgGetParameterType (param);
   if ( type == CG_SAMPLERRECT || 
        type == CG_SAMPLERCUBE ||
        type == CG_SAMPLER1D   ||
        type == CG_SAMPLER2D   ||
        type == CG_SAMPLER3D )
   {
      cgGLSetTextureParameter (param, val);
   }
   else
      cgGLSetParameter1f (param, (float)val);
}

void CGPath::setUniform3f (int index, float x, float y, float z)
{
   CGparameter param = _userparams[index];
   cgGLSetParameter3f (param, x, y, z);
}

void CGPath::setUniform4f (int index, float x, float y, float z, float w)
{
   CGparameter param = _userparams[index];
   cgGLSetParameter4f (param, x, y, z, w);
}

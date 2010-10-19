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

#include "cgengine.h"
#include "console.h"
#include "defines.h"

CGPath::CGPath(): 
   CodePath(),
   vp(0),
   fp(0),
   _mvpMatrix(0),
   _userparams()
{
}

CGPath::~CGPath()
{
}

bool CGPath::load (const std::string& vertex, const std::string& fragment)
{
   return loadVertexProgram(vertex) && loadFragmentProgram(fragment);
}

bool CGPath::loadVertexProgram(const std::string& vertexfile)
{
   CGEngine& cg = CGEngine::getInstance();

   vp = cgCreateProgramFromFile(cg.context(), 
                                CG_SOURCE, 
                                vertexfile.c_str(), 
                                cg.getVertexProfile(), 
                                "main", 
                                cg.getVertexProfileArguments());

   if (vp == NULL)
   {
      CGCompileError(cg.context(), "vertex", vertexfile);
      return false;
   }

   cgGLLoadProgram (vp);

   if ( cg.needsStateInfo() )
   {
      _mvpMatrix = cgGetNamedParameter(vp, "modelviewproj");
      ASSERT(_mvpMatrix != 0)
   }

   return true;
}

bool CGPath::loadFragmentProgram(const std::string& fragmentfile)
{
   CGEngine& cg = CGEngine::getInstance();
   
   fp = cgCreateProgramFromFile(cg.context(), 
                                CG_SOURCE,
                                fragmentfile.c_str(), 
                                cg.getFragmentProfile(), 
                                "main", 
                                NULL);

   if ( fp == NULL )
   {
      CGCompileError(cg.context(), "fragment", fragmentfile);
      return false;
   }

   cgGLLoadProgram (fp);

   return true;
}

void CGPath::CGCompileError(CGcontext context, const std::string& type, const std::string& file)
{
   Console& console = Console::getInstance();
   const char* list = cgGetLastListing(context);

   console.printf ("CG Path - Could not load %s program: %s", type.c_str(), file.c_str());
   if ( list != NULL )
      console.printf("%s", list);
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
   CGEngine& cg = CGEngine::getInstance();

   if ( vp != 0 )
   {
      cgGLEnableProfile( cg.getVertexProfile() );
      cgGLBindProgram( vp );

      if ( _mvpMatrix != 0 )
      {
         cgGLSetStateMatrixParameter(_mvpMatrix, CG_GL_MODELVIEW_PROJECTION_MATRIX, CG_GL_MATRIX_IDENTITY);
      }
   }

   if ( fp != 0 )
   {
      cgGLEnableProfile( cg.getFragmentProfile() );
      cgGLBindProgram( fp );
   }
}

void CGPath::disable() const
{
   if ( vp != 0 )
      cgGLDisableProfile( CGEngine::getInstance().getVertexProfile() );

   if ( fp != 0 )
      cgGLDisableProfile( CGEngine::getInstance().getFragmentProfile() );
}

CodePath::PathType CGPath::getType() const
{
   return CodePath::ECG;
}

int CGPath::getUniformLocation(const char* name) const
{
   CGparameter param = 0;

   if ( vp != NULL )
      param = cgGetNamedParameter(vp, name);

   if ( fp != NULL && param == 0 )
      param = cgGetNamedParameter(fp, name);

   _userparams.push_back(param);
   return _userparams.size() - 1;
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

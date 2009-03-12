/***************************************************************************
 *   Copyright (C) 2004 by Jeroen Broekhuizen                              *
 *   jeroen@nwnx.homedns.org                                               *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#include "console.h"

#include "cgengine.h"
#ifndef JENGINE_INLINE
#  include "cgengine.inl"
#endif

static void CGEngine_ErrorHandler(CGcontext context, CGerror error, void *data)
{
   CGEngine* pengine = reinterpret_cast<CGEngine*>(data);
   pengine->handleError(context, error);
}

CGEngine::CGEngine():
   cgContext(0),
   vertexProfile(CG_PROFILE_ARBVP1),
   fragmentProfile(CG_PROFILE_ARBFP1),
   _supportsVertex(true),
   _supportsFragment(true),
   _needsStateInfo(false)
{
}

CGEngine& CGEngine::getInstance()
{
	static CGEngine engine;
	return engine;
}

bool CGEngine::initialize()
{
	// set the error handler
   cgSetErrorHandler(CGEngine_ErrorHandler, this);

	// try to create the context
	cgContext = cgCreateContext ();
	if (cgContext == NULL)
   {
      Console::getLog() << "CG Error - Could not create a context.\n";
		return false;
   }

	// select appropriate vertex profile
	vertexProfile = CG_PROFILE_ARBVP1;
	if ( !cgGLIsProfileSupported(vertexProfile) )
   {
		vertexProfile = cgGLGetLatestProfile(CG_GL_VERTEX);
		if ( !cgGLIsProfileSupported(vertexProfile) )
      {
         Console::getLog() << "CG Warning - Vertex program is not supported on this system.\n";
         supportsVertexShader(false);
      }
      else
         needsStateInfo(true);
	}

   if ( supportsVertexShader() )
      cgGLSetOptimalOptions(vertexProfile);

	// select a fragment profile
	fragmentProfile = CG_PROFILE_ARBFP1;
	if ( !cgGLIsProfileSupported(fragmentProfile) )
   {
		fragmentProfile = cgGLGetLatestProfile(CG_GL_FRAGMENT);
		if ( !cgGLIsProfileSupported(fragmentProfile) )
      {
         Console::getLog() << "CG Warning - Fragment program is not supported on this system.\n";
         supportsFragmentShader(false);
      }
	}
   
   if ( supportsFragmentShader() )
      cgGLSetOptimalOptions(fragmentProfile);

	return true;
}

void CGEngine::handleError(CGcontext& context, CGerror error)
{
   ASSERT_MSG(context == cgContext, "Error of unknown context!")
   Console::getInstance().printf("%s\n", cgGetErrorString(error));
}

const char** CGEngine::getVertexProfileArguments()
{
   if ( !needsStateInfo() )
   {
      static const char* arguments[] = { "-DPROFILE_ARBVP1=true", NULL };
      return (const char**)arguments;
   }

   return NULL;
}

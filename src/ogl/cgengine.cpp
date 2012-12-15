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
#include "cgengine.h"
#ifndef JENGINE_INLINE
#  include "cgengine.inl"
#endif

#include "core/log/log.h"

using namespace Graphics;

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
   _supportsFragment(true)
{
}

bool CGEngine::initialize()
{
	// set the error handler
   cgSetErrorHandler(CGEngine_ErrorHandler, this);

	// try to create the context
	cgContext = cgCreateContext ();
	if (cgContext == NULL)
   {
      Log::getInstance().error("CG Error - Could not create a context");
		return false;
   }

	// select appropriate vertex profile
   vertexProfile = cgGLGetLatestProfile(CG_GL_VERTEX);
	if ( !cgGLIsProfileSupported(vertexProfile) )
   {
		Log::getInstance().error("CG Warning - Vertex program is not supported on this system.");
      supportsVertexShader(false);
	}

   if ( supportsVertexShader() )
      cgGLSetOptimalOptions(vertexProfile);

	// select a fragment profile
	fragmentProfile = cgGLGetLatestProfile(CG_GL_FRAGMENT);
	if ( !cgGLIsProfileSupported(fragmentProfile) )
   {
		Log::getInstance().warning("CG Warning - Fragment program is not supported on this system.");
      supportsFragmentShader(false);
	}
   
   if ( supportsFragmentShader() )
      cgGLSetOptimalOptions(fragmentProfile);

	return true;
}

void CGEngine::handleError(CGcontext& context, CGerror error)
{
   ASSERT_MSG(context == cgContext, "Error of unknown context!")
   Log::getInstance().error("%s", cgGetErrorString(error));
}

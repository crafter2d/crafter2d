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
#ifndef CG_ENGINE_H_
#define CG_ENGINE_H_

#include <GL/GLee.h>
#include <Cg/cg.h>
#include <Cg/cgGL.h>

class CGEngine
{
public:
	static CGEngine&  getInstance();

	bool              initialize();

   bool              hasContext();
	CGcontext         context();

	CGprofile         getVertexProfile();
   const char**      getVertexProfileArguments();
	CGprofile         getFragmentProfile();

   void              handleError(CGcontext& context, CGerror error);

   bool              supportsVertexShader() const;
   bool              supportsFragmentShader() const;
   bool              needsStateInfo() const;

private:
	                  CGEngine();
   void              operator=(const CGEngine& e);

   void              supportsVertexShader(bool support);
   void              supportsFragmentShader(bool support);
   void              needsStateInfo(bool needs);

private:
	CGcontext cgContext;
	CGprofile vertexProfile;
   CGprofile fragmentProfile;
   bool        _supportsVertex;
   bool        _supportsFragment;
   bool        _needsStateInfo;
};

#ifdef JENGINE_INLINE
#  include "cgengine.inl"
#endif

#endif

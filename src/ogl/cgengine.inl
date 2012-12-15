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
#include "core/defines.h"

namespace Graphics
{

INLINE bool CGEngine::isValid() const
{
   return hasContext();
}

INLINE bool CGEngine::hasContext() const
{
   return cgContext != 0;
}

INLINE CGcontext CGEngine::getContext()
{
   return cgContext;
}

INLINE CGprofile CGEngine::getVertexProfile()
{
   return vertexProfile;
}

INLINE CGprofile CGEngine::getFragmentProfile()
{
   return fragmentProfile;
}

INLINE bool CGEngine::supportsVertexShader() const
{
   return _supportsVertex;
}

INLINE void CGEngine::supportsVertexShader(bool support)
{
   _supportsVertex = support;
}

INLINE bool CGEngine::supportsFragmentShader() const
{
   return _supportsFragment;
}

INLINE void CGEngine::supportsFragmentShader(bool support)
{
   _supportsFragment = support;
}

};
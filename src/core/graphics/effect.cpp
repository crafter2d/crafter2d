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
#include "effect.h"
#ifndef JENGINE_INLINE
#  include "effect.inl"
#endif

#include "core/log/log.h"

#include "effecttechnique.h"
#include "blendstate.h"
#include "rendercontext.h"

namespace Graphics
{

/*!
    \fn Effect::Effect(void)
	 \brief initializes internal member variables (by default texture combiners are not used).
 */
Effect::Effect():
   mName(),
   mTechniques(),
   mpActiveTechnique(NULL),
   mpBlendState(NULL)
{
}

/*!
    \fn Effect::~Effect(void)
	 \brief releases internal member variables.
 */
Effect::~Effect()
{
	destroy();
}

/*!
    \fn Effect::destroy()
	 \brief Releases the codepath and the data for the stages. After calling this function you must
	 call load before enabling it again.
 */
void Effect::destroy()
{
	setBlendState(NULL);

   for ( std::size_t index = 0; index < mTechniques.size(); ++index )
   {
      delete mTechniques[index];
   }
   mTechniques.clear();
}

void Effect::addTechnique(EffectTechnique* ptechnique)
{
   ASSERT_PTR(ptechnique);

   mTechniques.push_back(ptechnique);

   if ( mpActiveTechnique == NULL )
   {
      mpActiveTechnique = ptechnique;
   }
}

/// \fn Effect::setBlendState(BlendState* pblendstate)
/// \brief Set the new blendstate object used during rendering
void Effect::setBlendState(BlendState* pblendstate)
{
   delete mpBlendState;
   mpBlendState = pblendstate;
}

// - Buffers

UniformBuffer* Effect::createUniformBuffer(const String& name) const
{
   ASSERT_PTR(mpActiveTechnique);
   return mpActiveTechnique->createUniformBuffer(name);
}

VertexBuffer* Effect::createVertexBuffer(Device& device, int length, int usage)
{
   ASSERT_PTR(mpActiveTechnique);
   return mpActiveTechnique->createVertexBuffer(device, length, usage);
}

// - Operations

/*!
    \fn Effect::enable()
	 \brief Enables the textures, shaders and if neccessary the texture combiners.
 */
void Effect::enable(RenderContext& context) const
{
   context.setBlendState(*mpBlendState);
   
   ASSERT_PTR(mpActiveTechnique);
   mpActiveTechnique->enable(context);
}

} // end namespace

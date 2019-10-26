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
#ifndef EFFECT_H_
#define EFFECT_H_

#include <vector>

#include "core/content/content.h"
#include "core/string/string.h"

class TiXmlElement;

namespace Graphics
{
   class BlendState;
   class EffectTechnique;
   class Device;
   class RenderContext;
   class UniformBuffer;
   class VertexBuffer;

   /**
    * @author Jeroen Broekhuizen
    * \brief Implements an effect used while rendering.
    *
    * This class implements the an effect for use during rendering of objects. 
    */
   class CORE_API Effect : public IContent
   {
   public:
    // types
      using Techniques = std::vector<EffectTechnique*>;

      Effect();
	   virtual ~Effect();

    // get/set
      const String& getName() const {
         return mName;
      }
      const Techniques& getTechniques() const {
         return mTechniques;
      }

    // buffers
      UniformBuffer* createUniformBuffer(const String& name) const;
      VertexBuffer*  createVertexBuffer(Device& device, int length, int usage);
	
    // operations
      void addTechnique(EffectTechnique* ptechnique);

      void setBlendState(BlendState* pblendstate);
      void setActiveTechnique(const String& name);
      void setConstantBuffer(RenderContext& context, int index, const UniformBuffer& buffer);

      void enable(RenderContext& context) const;
	
   private:
    // operations
      void destroy();

    // data
      String            mName;
      Techniques        mTechniques;
      EffectTechnique*  mpActiveTechnique;
      BlendState*       mpBlendState;
   };
}

#endif

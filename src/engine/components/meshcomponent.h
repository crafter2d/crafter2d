
#ifndef MESH_COMPONENT_H
#define MESH_COMPONENT_H

#include "core/math/xform.h"
#include "core/string/string.h"

#include "component.h"

namespace Graphics
{
   class RenderContext;
   class Sprite;
}

class MeshComponent : public Component
{
public:
   MeshComponent();
   virtual ~MeshComponent();

 // get/set
   const Vector& getPosition() const;
   float         getAngle() const;

   const Graphics::Sprite& getSprite() const;
   void                    setSprite(Graphics::Sprite* psprite);

 // operations
   void render(Graphics::RenderContext& context) const;

 // overloads
   virtual void registerComponent(Components& components);
   virtual void handleMessage(ComponentMessage& message);
   virtual void update(float delta);

private:
 // operations
   void updateBuffers(Graphics::RenderContext& context);

 // members
   XForm                         mTransform;
   Graphics::Sprite*             mpSprite;
};

#ifdef JENGINE_INLINE
#  include "meshcomponent.inl"
#endif

#endif // MESH_COMPONENT_H

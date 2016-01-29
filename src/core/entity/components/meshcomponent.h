
#ifndef MESH_COMPONENT_H
#define MESH_COMPONENT_H

#include "core/defines.h"
#include "core/math/xform.h"
#include "core/string/string.h"

#include "component.h"

namespace Graphics
{
   class RenderContext;
}

namespace c2d
{
   class Sprite;
}

class CORE_API MeshComponent : public Component
{
public:
   MeshComponent();
   virtual ~MeshComponent();

 // get/set
   const Vector& getPosition() const;
   float         getAngle() const;

   const c2d::Sprite& getSprite() const {
      ASSERT_PTR(mpSprite);
      return *mpSprite;
   }

   void setSprite(c2d::Sprite* psprite) {
      mpSprite = psprite;
   }

 // operations
   void render(Graphics::RenderContext& context) const;

 // overloads
   virtual void registerComponent(Components& components);
   virtual void handleMessage(ComponentMessage& message);
   virtual void update(float delta);

private:

 // members
   XForm        mTransform;
   c2d::Sprite* mpSprite;
};

#endif // MESH_COMPONENT_H

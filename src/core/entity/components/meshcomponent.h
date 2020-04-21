
#ifndef MESH_COMPONENT_H
#define MESH_COMPONENT_H

#include "core/defines.h"
#include "core/graphics/sprites/sprite.h"
#include "core/math/xform.h"
#include "core/string/string.h"

#include "component.h"

namespace Graphics
{
   class RenderContext;
}

class CORE_API MeshComponent final : public Component
{
public:
   explicit MeshComponent(c2d::Sprite&& sprite);
   virtual ~MeshComponent();

 // get/set
   const Vector& getPosition() const;
   float         getAngle() const;

   const c2d::Sprite& getSprite() const {
      return mSprite;
   }
   
 // operations
   void render(Graphics::RenderContext& context) const;

 // overloads
   virtual void registerComponent(Components& components);
   virtual void handleMessage(ComponentMessage& message);
   virtual void update(float delta);

private:

 // members
   XForm       mTransform;
   c2d::Sprite mSprite;
};

#endif // MESH_COMPONENT_H


#pragma once

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
   const c2d::Sprite& getSprite() const { return mSprite; }
   c2d::Sprite& getSprite() { return mSprite; }
   
 // operations
   void render(Graphics::RenderContext& context) const;

 // overloads
   virtual void registerComponent(Components& components);
   virtual void handleMessage(ComponentMessage& message);
   virtual void update(float delta);

private:

 // members
   c2d::Sprite mSprite;
};

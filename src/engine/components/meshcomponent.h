
#ifndef MESH_COMPONENT_H
#define MESH_COMPONENT_H

#include "core/math/xform.h"
#include "core/string/string.h"

#include "component.h"

namespace Graphics
{
   class RenderContext;
}

class Animator;
class Sprite;

class MeshComponent : public Component
{
public:
   MeshComponent();
   virtual ~MeshComponent();

 // get/set
   const Animator& getAnimator() const;
   void            setAnimator(Animator* panimator);

   const Vector& getPosition() const;
   float         getAngle() const;

   const Sprite& getSprite() const;
   void          setSprite(Sprite* psprite);

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
   Animator*                     mpAnimator; // owned
   Sprite*                       mpSprite;
};

#ifdef JENGINE_INLINE
#  include "meshcomponent.inl"
#endif

#endif // MESH_COMPONENT_H

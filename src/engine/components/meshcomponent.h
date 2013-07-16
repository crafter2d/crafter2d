
#ifndef MESH_COMPONENT_H
#define MESH_COMPONENT_H

#include "core/graphics/effect.h"
#include "core/math/size.h"
#include "core/math/xform.h"
#include "core/string/string.h"

#include "component.h"

namespace Graphics
{
   class Device;
   class RenderContext;
   class IndexBuffer;
   class VertexBuffer;
};

class Animator;

class MeshComponent : public Component
{
public:
   MeshComponent();
   virtual ~MeshComponent();

 // get/set
   const Animator& getAnimator() const;
   void            setAnimator(Animator* panimator);

   const String& getEffectName() const;
   void          setEffectName(const String& name);

   const Vector& getPosition() const;
   float         getAngle() const;

   const Size& getSize() const;
   void        setSize(const Size& size);

 // operations
   void initialize(Graphics::Device& device, Graphics::RenderContext& context);
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
   String                        mEffectName;
   Graphics::Effect              mEffect;
   Graphics::VertexBuffer*       mpVertexBuffer;
   Graphics::IndexBuffer*        mpIndexBuffer;
   Size                          mSize;
   Size                          mHalfSize;
};

#ifdef JENGINE_INLINE
#  include "meshcomponent.inl"
#endif

#endif // MESH_COMPONENT_H


#ifndef MESH_COMPONENT_H
#define MESH_COMPONENT_H

#include "core/math/size.h"
#include "core/string/string.h"

#include "engine/effect.h"

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

   const Size& getSize() const;
   void        setSize(const Size& size);

 // operations
   void initialize(Graphics::Device& device);
   void render(Graphics::RenderContext& context);

 // overloads
   virtual void handleMessage(const ComponentMessage& message);
   virtual void update(float delta);

private:
 // operations
   void updateBuffers();

 // members
   Animator*               mpAnimator; // owned
   Effect                  mEffect;
   String                  mEffectName;
   Graphics::VertexBuffer* mpVertexBuffer;
   Graphics::IndexBuffer*  mpIndexBuffer;
   int                     mVertexFormat;
   Size                    mSize;
   Size                    mHalfSize;
};

#ifdef JENGINE_INLINE
#  include "meshcomponent.inl"
#endif

#endif // MESH_COMPONENT_H

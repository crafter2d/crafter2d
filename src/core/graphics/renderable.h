#ifndef RENDERABLE_H
#define RENDERABLE_H

namespace Graphics
{
   class RenderContext;
   class Viewport;

   class Renderable
   {
   public:

      virtual void viewportChanged(RenderContext& context, const Viewport& viewport) = 0;

      virtual void update(RenderContext& context, float delta) = 0;
      virtual void render(RenderContext& context) = 0;
   };
}

#endif // RENDERABLE_H


#ifndef EFFECT_BUILDER_H
#define EFFECT_BUILDER_H

class ASTEffect;

namespace Graphics
{
   class Effect;
}

class EffectBuilder
{
public:

   Graphics::Effect* build(const ASTEffect& effect);
};

#endif // EFFECT_BUILDER_H

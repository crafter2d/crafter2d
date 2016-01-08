#include "spritecomponent.h"

SpriteComponent::SpriteComponent()
{

}

void SpriteComponent::addAnimation(SpriteAnimation&& that)
{
    mAnimations.append(that);
}

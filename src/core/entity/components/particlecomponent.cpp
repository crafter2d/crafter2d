
#include "particlecomponent.h"

#include "core/entity/entity.h"
#include "core/graphics/rendercontext.h"
#include "core/graphics/particles/particlesystem.h"
#include "core/entity/componentmessages/componentmessage.h"

using namespace Graphics;

ParticleComponent::ParticleComponent():
   Component(ComponentInterface::eParticleComponent),
   mpSystem(nullptr)
{
}

// - Get/set

void ParticleComponent::setParticleSystem(ParticleSystem* psystem)
{
   mpSystem = psystem;
}


// overrides
   
void ParticleComponent::registerComponent(Components& components)
{
   Component::registerComponent(components);

   components.subscribeMessageType(*this, ComponentInterface::ePositionChangedMsg);
   components.subscribeMessageType(*this, ComponentInterface::eUpdatedMsg);
   components.subscribeMessageType(*this, ComponentInterface::eUpdateMsg);
   components.subscribeMessageType(*this, ComponentInterface::eRenderMsg);
}

void ParticleComponent::handleMessage(ComponentMessage& message)
{
   using namespace ComponentInterface;

	switch ( message.getMessageType() )
	{
   case ePositionChangedMsg:
      {
         mpSystem->setPosition(getEntity().getPosition());
      }
      break;
   case eUpdatedMsg:
      {
      mpSystem->setPosition(getEntity().getTransform().getPosition());
      }
      break;
   case eUpdateMsg:
      {
         float* pdelta = static_cast<float*>(message.getData());
         update(*pdelta);
      }
      break;
   case eRenderMsg:
      {
         RenderContext* pcontext = static_cast<RenderContext*>(message.getData());
         render(*pcontext);
      }
      break;
   default:
      break;
   }
}

// - Operations

void ParticleComponent::update(float delta)
{
   mpSystem->update(delta);
}

void ParticleComponent::render(Graphics::RenderContext& context)
{
   mpSystem->draw(context);
}

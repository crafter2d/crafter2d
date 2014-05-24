
package demo;

use demo.*;
use engine.core.*;
use engine.game.*;
use engine.messages.*;

class DemoActionMap extends ActionMap
{
	GameServer mServer;
	
	public DemoActionMap(GameServer server)
	{
		super();
		
		mServer = server;
		
		setProcess(server);
	}
	
	public void onKeyDown(Entity object, int action)
	{
		ActionMessage message = new ActionMessage(object.getId(), action);
		mServer.broadcastMessage(message);
		
		if ( action == 3 ) // jump
		{
			object.jump();
		}
		else
		{
			InputForceGenerator force = (InputForceGenerator) object.getForceGenerator();
			Vector2D velocity = force.getVelocity();
		
			switch ( action )
			{
				case 1: // move left
				{
					velocity.setX(-20.0);
					break;
				}
				case 2: // move right
				{
					velocity.setX(20.0);
					break;
				}
			}
			
			object.setAnimation(2);
			force.setVelocity(velocity);
		}
	}
	
	public void onKeyUp(Entity object, int action)
	{
		InputForceGenerator force = (InputForceGenerator) object.getForceGenerator();
		Vector2D velocity = force.getVelocity();
		
		if ( action == 1 || action == 2 )
		{
			velocity.setX(0.0);
		}
		
		object.setAnimation(1);
		force.setVelocity(velocity);
	}
}


package engine.game;

use engine.core.*;

class ActionMap
{
	public native ActionMap();
	
	// - Calls
	
	public void onSwapLeakDetector(boolean down)
	{
		if ( down ) // execute only once
		{
			getProcess().swapLeakDetection();
		}
	}
	
	public void onKeyDown(Actor object, int action)
	{
		Vector2D velocity = object.getVelocity();
		
		if ( action == 3 ) // jump
		{
			object.jump();
		}
		else
		{
			switch ( action )
			{
				case 1: // move left
				{
					velocity.setX(-20.0);
					
					if ( object.direction() )
					{
						object.flip();
					}
					break;
				}
				case 2: // move right
				{
					velocity.setX(20.0);
					
					if ( !object.direction() )
					{
						object.flip();
					}
					break;
				}
			}
			
			object.setAnimation(2);
			object.getForceGenerator().setVelocity(velocity);  
			
			// <-- here the virtualobject creature created by the client should be found! 
			// otherwise the force generator will be non-existing...
		}
	}
	
	public void onKeyUp(Actor object, int action)
	{
		Vector2D velocity = object.getVelocity();
		
		if ( action == 1 || action == 2 )
		{
			velocity.setX(0.0);
		}
		
		object.setAnimation(1);
		object.getForceGenerator().setVelocity(velocity);
	}
	
	// - Natives
	
	public native Process getProcess();
	public native void setProcess(Process process);
	public native void bind(int action, string function);
}

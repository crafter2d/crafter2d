
package demo;

use engine.core.*;
use engine.game.*;

class DemoClientActionMap extends ActionMap
{
	public DemoClientActionMap()
	{
		bind(6, "onSwapLeakDetector");
	}

	public void onKeyDown(Actor object, int action)
	{
		if ( action == 3 )
		{
			object.jump();
		}
		else
		{
			InputForceGenerator force = (InputForceGenerator) object.getForceGenerator();
			Vector2D velocity = force.getVelocity();
			
			switch ( action )
			{
				case 1:
					velocity.setX(-20.0);
						
					if ( object.direction() )
					{
						object.flip();
					}
					break;
				case 2:
					velocity.setX(20.0);
						
					if ( !object.direction() )
					{
						object.flip();
					}
					break;
			}
			
			force.setVelocity(velocity);
		}
	}

	public void onKeyUp(Actor object, int action)
	{
	}
}

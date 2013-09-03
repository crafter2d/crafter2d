
package demo;

use engine.core.*;
use engine.game.*;
use box2d.*;

class Bridge
{
	public void create(Process process, Vector2D left, Vector2D right)
	{
		Box2DSimulator simulator = process.getWorld().getSimulator();
				
		Vector2D anchorpos = new Vector2D();
		anchorpos.setX(left.x + 6.0);
		anchorpos.setY(left.y);
		
		real width = 12.0;
		real diff = right.x - left.x;
		real num = Math.ceil(diff / width);
      
		Box2DBody leftbody = createSide(process, left, 0);
		Box2DBody rightbody = null;
		
		int count = (int)num;
		for ( int index = 1; index < count; ++index )
		{
			left.x = left.x + width;
			rightbody = createPart(process, left, index);
			
			simulator.createRevoluteJoint(leftbody, rightbody, anchorpos);
			
			leftbody = rightbody;
			anchorpos.x = anchorpos.x + width;
		}
		
		left.x = left.x + width;
		rightbody = createSide(process, left, count+1);
		simulator.createRevoluteJoint(leftbody, rightbody, anchorpos);
	}
	
	private Box2DBody createSide(Process process, Vector2D position, int index)
	{
		Actor box = createBox(process, position, "bridgeside", index);
		
		return box.getBody();
	}
	
	private Box2DBody createPart(Process process, Vector2D position, int index)
	{
		Actor box = createBox(process, position, "bridgepart", index);
		
		return box.getBody();
	}
	
	private Actor createBox(Process process, Vector2D position, string name, int index)
	{
		return Actor.construct(process, name + index, position, "../objects/" + name + ".xml");
	}
}

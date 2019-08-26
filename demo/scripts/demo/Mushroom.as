
package demo;

use engine.game.*;
use box2d.box2dbody;

// Mushroom

class Mushroom extends Entity
{
	
	public void onCreated()
	{
	}

	public void collide(Object target, int side, boolean begin)
	{
		super.collide(target, side, begin);
	}
}

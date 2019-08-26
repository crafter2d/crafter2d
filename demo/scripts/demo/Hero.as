
package demo;

use engine.game.*;

// Hero class

class Hero extends Entity
{
	public void onCreated()
	{
		setInitialFaceDirection(Entity.FACE_RIGHT);
	}
	
	public void collide(Object target, int side, boolean begin)
	{
		super.collide(target, side, begin);
		
		if ( target instanceof Mushroom )
		{
			Entity entity = (Entity)target;
			entity.remove();
		}
	}
}

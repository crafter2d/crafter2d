
package demo;

use engine.game.*;

// Hero class

class Hero extends Entity
{
	public void onCreated()
	{
		setInitialFaceDirection(Entity.FACE_RIGHT);
	}
}

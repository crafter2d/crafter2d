
package demo;

use engine.game.*;

// Hero class

class Hero extends Actor
{
	public void onCreated()
	{
		setInitialFaceDirection(Actor.FACE_RIGHT);
	}
}
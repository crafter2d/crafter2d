
package demo;

use engine.game.*;
use box2d.box2dbody;

// grudge enemy

class Grudge extends Actor
{
	private static int WEAPON_CLUB = 1;
	private static int WEAPON_SPEAR = 2;
	
	private int mWeapon;
	
	public void onCreated()
	{
		mWeapon = WEAPON_CLUB;
		
		setState(new PatrolState());
		//setForceGenerator(new InputForceGenerator());
		setInitialFaceDirection(Actor.FACE_LEFT);
	}

	public void collide(Object target, int side, boolean begin)
	{
		super.collide(target, side, begin);
		
		if ( target instanceof Bound )
		{
         if ( getState() instanceof PatrolState )
         {
            if ( (side == 2 && isLookingLeft()) || (side == 3 && isLookingRight()) )
            {
               turnAround();
            }
            
            if ( isOnGround() )
            {
               // when on the ground, start the walking animation
               setAnimation(2);
            }
         }
		}
	}
	
	// - State
	
	public AttackState getAttackState()
	{
		if ( mWeapon == WEAPON_CLUB )
		{
			return new ClubAttackState();
		}
		
		return null;
	}
}

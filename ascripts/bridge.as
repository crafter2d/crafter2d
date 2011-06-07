
use box2d.*;

use vector2d;
use world;

class Bridge
{
	public void create(World world, Vector2D left, Vector2D right)
	{
		Box2DSimulator simulator = world.getSimulator();
		
		Vector2D pos = new Vector2D();
		pos.setX(left.getX() + 6.0);
		pos.setY(left.getY());

		Box2DRevoluteJointDefinition jointdef = new Box2DRevoluteJointDefinition();
		jointdef.setLeft(createSide(world, left, 0));
		jointdef.setAnchor(pos);
		
		real diff = right.getX() - left.getX();
		real num = diff / 12.0;
		real x = left.getX() + 12.0;
		
		int count = (int)num;
		System.console.println("Num = " + count);
		for ( int index = 1; index < count; ++index )
		{
			pos.setX(x);
			
			jointdef.setRight(createPart(world, pos, index));
			
			simulator.createRevoluteJoint(jointdef);
			
			x = x + 12;
			
			jointdef.setLeft(jointdef.getRight());
			jointdef.getAnchor().offsetX(12.0);
		}
		
		jointdef.setRight(createSide(world, right, (int) num+1));
		simulator.createRevoluteJoint(jointdef);
	}
	
	private Box2DBody createSide(World world, Vector2D position, int index)
	{
		Creature box = createBox(world, position, "bridgeside", index);
		
		return box.getBody();
	}
	
	private Box2DBody createPart(World world, Vector2D position, int index)
	{
		Creature box = createBox(world, position, "bridgepart", index);
		
		return box.getBody();
	}
	
	private Creature createBox(World world, Vector2D position, string name, int index)
	{
		return Creature.construct(world, name + index, position, "../objects/" + name + ".xml");
	}
}

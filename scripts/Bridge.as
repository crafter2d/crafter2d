
use box2d.*;

class Bridge
{
	public void create(Process process, Vector2D left, Vector2D right)
	{
		Box2DSimulator simulator = process.getWorld().getSimulator();
		
		Vector2D pos = new Vector2D();
		pos.setX(left.getX() + 6.0);
		pos.setY(left.getY());

		Box2DRevoluteJointDefinition jointdef = new Box2DRevoluteJointDefinition();
		jointdef.setLeft(createSide(process, left, 0));
		jointdef.setAnchor(pos);
		
		real diff = right.getX() - left.getX();
		real num = diff / 12.0;
		real x = left.getX() + 12.0;
		
		int count = (int)num;
		for ( int index = 1; index < count; ++index )
		{
			pos.setX(x);
			
			jointdef.setRight(createPart(process, pos, index));
			
			simulator.createRevoluteJoint(jointdef);
			
			x = x + 12;
			
			jointdef.setLeft(jointdef.getRight());
			jointdef.getAnchor().offsetX(12.0);
		}
		
		jointdef.setRight(createSide(process, right, (int) num+1));
		simulator.createRevoluteJoint(jointdef);
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

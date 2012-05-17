
package demo;

use box2d.*;

class Bridge
{
	public void create(Process process, Vector2D left, Vector2D right)
	{
		Box2DSimulator simulator = process.getWorld().getSimulator();
		
		// the position is center of the bridge part
		
		Vector2D pos = new Vector2D();
		pos.setX(left.getX() + 6.0);
		pos.setY(left.getY());
		
		Vector2D localanchorleft = new Vector2D();
		localanchorleft.set(6, 0);
		Vector2D localanchorright = new Vector2D();
		localanchorright.set(-6, 0);

		Box2DRevoluteJointDefinition jointdef = new Box2DRevoluteJointDefinition();
		jointdef.setLeft(createSide(process, left, 0));
		jointdef.setAnchor(pos);
		
		Box2DRopeJointDefinition ropejointdef = new Box2DRopeJointDefinition();
		ropejointdef.setLocalAnchorLeft(localanchorleft);
		ropejointdef.setLocalAnchorRight(localanchorright);
		ropejointdef.setLeft(jointdef.getLeft());
		
		real diff = right.getX() - left.getX();
		real num = diff / 12.0;
		real x = left.getX() + 12.0;
		
		int count = (int)num;
		for ( int index = 1; index < count; ++index )
		{
			pos.setX(x);
			
			Box2DBody part = createPart(process, pos, index);
			
			jointdef.setRight(part);
			simulator.createRevoluteJoint(jointdef);
			jointdef.setLeft(part);
			jointdef.getAnchor().offsetX(12.0);
			
			ropejointdef.setRight(part);
			simulator.createRopeJoint(ropejointdef);
			ropejointdef.setLeft(part);			
			
			x = x + 12;
		}
		
		Box2DBody rightside = createSide(process, right, count+1);
		
		jointdef.setRight(rightside);
		simulator.createRevoluteJoint(jointdef);
		
		ropejointdef.setRight(rightside);
		simulator.createRopeJoint(ropejointdef);
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

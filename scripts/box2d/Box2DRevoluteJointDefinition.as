
package box2d;

use engine.core.*;

public class Box2DRevoluteJointDefinition
{
	private Box2DBody mLeft;
	private Box2DBody mRight;
	private Vector2D  mAnchor;
	
	public Box2DRevoluteJointDefinition()
	{
		super();
	}
		
	// Get/set
	
	public Box2DBody getLeft()
	{
		return mLeft;
	}
	
	public void setLeft(Box2DBody body)
	{
		mLeft = body;
	}
	
	public Box2DBody getRight()
	{
		return mRight;
	}
	
	public void setRight(Box2DBody body)
	{
		mRight = body;
	}
	
	public Vector2D getAnchor()
	{
		return mAnchor;
	}
	
	public void setAnchor(Vector2D anchor)
	{
		mAnchor = anchor;
	}
}

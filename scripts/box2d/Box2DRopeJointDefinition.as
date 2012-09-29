
package box2d;

use engine.core.*;

public class Box2DRopeJointDefinition
{
	private Box2DBody mLeft;
	private Box2DBody mRight;
	private Vector2D  mLocalAnchorLeft;
	private Vector2D  mLocalAnchorRight;

	public Box2DRopeJointDefinition()
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
	
	public Vector2D getLocalAnchorLeft()
	{
		return mLocalAnchorLeft;
	}
	
	public void setLocalAnchorLeft(Vector2D anchor)
	{
		mLocalAnchorLeft = anchor;
	}
	
	public Vector2D getLocalAnchorRight()
	{
		return mLocalAnchorRight;
	}
	
	public void setLocalAnchorRight(Vector2D anchor)
	{
		mLocalAnchorRight = anchor;
	}
}
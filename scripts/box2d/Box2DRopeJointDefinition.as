
package box2d;

public class Box2DRopeJointDefinition
{
	public native Box2DRopeJointDefinition();
	
	public native Box2DBody getLeft();
	public native void setLeft(Box2DBody left);
	
	public native Box2DBody getRight();
	public native void setRight(Box2DBody right);
	
	public native Vector2D getLocalAnchorLeft();
	public native void setLocalAnchorLeft(Vector2D anchor);
	
	public native Vector2D getLocalAnchorRight();
	public native void setLocalAnchorRight(Vector2D anchor);
}
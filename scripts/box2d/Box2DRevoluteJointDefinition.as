
package box2d;

use engine.core.*;

public class Box2DRevoluteJointDefinition
{
	public native Box2DRevoluteJointDefinition();
	
	public native Box2DBody getLeft();
	public native void setLeft(Box2DBody left);
	
	public native Box2DBody getRight();
	public native void setRight(Box2DBody right);
	
	public native Vector2D getAnchor();
	public native void setAnchor(Vector2D anchor);
}

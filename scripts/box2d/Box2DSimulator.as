
package box2d;

use engine.core.*;

public class Box2DSimulator
{
   // Checks if there is line of sight between two actors
   public native boolean lineOfSight(Body from, Body to);
   
   // Creates a revolute joint. This type of joint can be used to freely rotate an object
   // around a point on another object.  
   public void createRevoluteJoint(Box2DBody left, Box2DBody right, Vector2D anchor)
   {
      createRevoluteJoint(left, right, anchor.x, anchor.y);
   }
   
   // Creates a rope joint. All it does is ensuring that two objects aren't stretched
   // beyond a predefined distance. To be used in combination with another joint.
   //public native void createRopeJoint(Box2DRopeJointDefinition definition);
   
   // - natives
   
   private native void createRevoluteJoint(Box2DBody left, Box2DBody right, real anchorx, real anchory);
}

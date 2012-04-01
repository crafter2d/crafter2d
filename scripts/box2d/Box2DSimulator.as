
package box2d;

public class Box2DSimulator
{
   // Checks if there is line of sight between two actors
   public native boolean lineOfSight(Body from, Body to);
   
   // Creates a revolute joint. This type of joint can be used to freely rotate an object
   // around a point on another object.
   public native void createRevoluteJoint(Box2DRevoluteJointDefinition definition);
   
   // Creates a rope joint. All it does is ensuring that two objects aren't stretched
   // beyond a predefined distance. To be used in combination with another joint.
   public native void createRopeJoint(Box2DRopeJointDefinition definition);
}

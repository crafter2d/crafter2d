
package box2d;

public class Box2DSimulator
{
   // test if there is line of sight between two actors
   public native boolean lineOfSight(Actor from, Actor to);
   
   public native void createRevoluteJoint(Box2DRevoluteJointDefinition definition);
   public native void createRopeJoint(Box2DRopeJointDefinition definition);
}

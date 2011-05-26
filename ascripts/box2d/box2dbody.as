
package box2d;

use inputforcegenerator;

class Box2DBody
{
	public native void addForceGenerator(InputForceGenerator generator);
	public native void generateSensors();
}

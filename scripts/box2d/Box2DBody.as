
package box2d;

use engine.game.inputforcegenerator;

class Box2DBody
{
	public native void addForceGenerator(InputForceGenerator generator);
	public native void generateSensors();
}

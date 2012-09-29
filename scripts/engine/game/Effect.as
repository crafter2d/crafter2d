
package engine.game;

use engine.core.*;

class Effect
{
	public Texture getDiffuseTexture()
	{
		return resolveTexture("diffuseMap");
	}
	
	private native Texture resolveTexture(string name);
}

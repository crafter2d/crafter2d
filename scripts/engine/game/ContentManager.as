
package engine.game;

final class ContentManager
{
	private ContentManager()
	{
		super();
	}
	
	public Entity loadEntity(string filename)
	{
		Entity result = native;
		result.onCreated();
		return result;
	}
	
	
	public native World load(string filename);
}

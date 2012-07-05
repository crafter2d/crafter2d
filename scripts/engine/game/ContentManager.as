
package engine.game;

final class ContentManager
{
	private ContentManager()
	{
		super();
	}
	
	public Entity loadEntity(string filename)
	{
		Entity result = native_loadEntity(filename);
		result.onCreated();
		return result;
	}
	
	
	public native World load(string filename);
	private native Entity native_loadEntity(string filename);
}

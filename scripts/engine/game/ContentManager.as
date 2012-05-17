
package engine.game;

final class ContentManager
{
	private ContentManager()
	{
		super();
	}
	
	public native Entity loadEntity(string filename);
	public native World load(string filename);
}

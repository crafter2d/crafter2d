
package engine.core;

final class Texture
{
	private Texture()
	{
		super();
	}
	
	public Size getSize()
	{
		return new Size(getWidth(), getHeight());
	}
		
	public native int getWidth();
	public native int getHeight();
}

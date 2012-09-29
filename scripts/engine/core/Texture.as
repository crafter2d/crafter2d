
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
		
	public native string getName();
	public native int getWidth();
	public native int getHeight();
	public native int getSourceWidth();
	public native int getSourceHeight();
}

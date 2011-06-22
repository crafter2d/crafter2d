
package engine.ui;

use engine.shapes.*;

final class Font
{
	private Font()
	{
		super();
	}
	
	public Rectangle getTextSize(string text)
	{
		Rectangle r = new Rectangle();
		r.width = native_textWidth(text);
		r.height = native_textHeight(text);
		return r;
	}
	
	private native int native_textHeight(string text);
	private native int native_textWidth(string text);
	
	public native int getBaseLine();
	public native void render(string text);
}

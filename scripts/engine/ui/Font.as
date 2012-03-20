
package engine.ui;

use engine.core.*;
use engine.shapes.*;

final class Font
{
	private Font()
	{
		super();
	}
	
	public Size getTextSize(string text)
	{
		Size result = new Size();
		result.width = native_textWidth(text);
		result.height = native_textHeight(text);
		return result;
	}
	
	private native int native_textHeight(string text);
	private native int native_textWidth(string text);
	
	public native int getBaseLine();
	public native void render(string text);
}

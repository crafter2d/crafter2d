
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
		result.width = getTextWidth(text);
		result.height = getTextHeight(text);
		return result;
	}
	
	private native int getTextHeight(string text);
	private native int getTextWidth(string text);
	
	public native int getBaseLine();
	public native void render(string text);
}

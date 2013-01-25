
package engine.ui;

use engine.game.*;

class FontManager
{
	static private FontManager mFontManager = new FontManager();
	
	private Process mProcess;
	
	static public FontManager getInstance()
	{
		return mFontManager;
	}
	
	void setProcess(Process process)
	{
		mProcess = process;
	}
	
	Font getFont(string name, int size)
	{
		return null; // mProcess.getFont(name, size);
	}
}

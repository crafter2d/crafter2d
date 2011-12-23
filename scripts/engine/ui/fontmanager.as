
package engine.ui;

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
		return mProcess.getFont(name, size);
	}
}

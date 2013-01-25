
package engine.core;

use engine.game.Process;

class TextureManager
{
	static private TextureManager mTextureManager = new TextureManager();
	
	private Process mProcess;
	
	static public TextureManager getInstance()
	{
		return mTextureManager;
	}
	
	void setProcess(Process process)
	{
		mProcess = process;
	}
	
	Texture getTexture(string name)
	{
		return null; // mProcess.getTexture(name);
	}
}

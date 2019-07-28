package ui;

use system.Client;
use engine.game.Renderable;

class YuiSystem implements Renderable
{
	public native YuiSystem();
	
	/// Initialize the UI system internals, should be called before any other functions are used
	public native boolean initialize(ContentManager contentmgr, ScriptManager scriptmgr, int width, int height);
	
	/// Load a UI configuration file
	public native YuiWindow load(string file);
	
	/// Set the theme that is user for drawing the UI
	public native void setTheme(string themefile);
	
	/// Gets the renderable to register as overlay with the engine
	public native Renderable getRenderable();
}

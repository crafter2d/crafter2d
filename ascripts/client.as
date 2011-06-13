
use system.*;
use engine.ui.*;

abstract class Client extends Process
{
	private EngineGraphics mGraphics = new EngineGraphics();
	
	public native Client()
	{
		super();
		
		FontManager mgr = FontManager.getInstance();
		mgr.setProcess(this);
	}
	
	private native void nativeRender(real delta);
	private native void nativeDisplay();
	
	public native void connect(string host, int port, string name);
	public native void update(real delta);
	public native void setWindow(GameWindow window);
	public native void setActionMap(ActionMap map);
	public native void setKeyMap(KeyMap map);
	public native boolean isActive();
	
	public boolean create()
	{
		if ( super.create() )
		{
			FontManager mgr = FontManager.getInstance();
			mgr.setProcess(this);
			
			return true;
		}
		return false;
	}
	
	public EngineGraphics getGraphics()
	{
		return mGraphics;
	}
	
	public void render(real delta)
	{
		nativeRender(delta);
		
		Canvas canvas = Canvas.getInstance();
		canvas.paint(mGraphics);
		
		nativeDisplay();
	}
}

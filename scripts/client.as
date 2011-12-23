
use system.*;
use engine.ui.*;
use engine.input.*;
use engine.core.*;

abstract class Client extends Process
{
	private EngineGraphics 			mGraphics = new EngineGraphics();
	private MouseEventDispatcher 	mMouseDispatcher;
	private KeyEventDispatcher		mKeyDispatcher;
	
	public native Client()
	{
		super();
		
		FontManager mgr = FontManager.getInstance();
		mgr.setProcess(this);
		
		TextureManager tmgr = TextureManager.getInstance();
		tmgr.setProcess(this);
		
		mMouseDispatcher = new MouseEventDispatcher();
		mKeyDispatcher = new KeyEventDispatcher();
	}
	
	private native void nativeRender(real delta);
	private native void nativeDisplay();
	private native void native_setWindow(GameWindow window);
	
	public native void connect(string host, int port, string name);
	public native void update(real delta);
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
	
	public void setWindow(GameWindow window)
	{
		native_setWindow(window);
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
	
	private void onKeyEvent(int key, boolean pressed)
	{
		mKeyDispatcher.dispatch(key, pressed);
	}
	
	private void onMouseEvent(int x, int y, int buttons, int event)
	{
		mMouseDispatcher.dispatch(x, y, buttons, event);
	}
}

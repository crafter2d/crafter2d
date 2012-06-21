
use system.gamewindowfactory;
use system.gamewindow;
use engine.game.*;
use engine.core.*;
use engine.xml.*;
use engine.io.*;

use demo.GameServer;

class Game
{
	private static string ServerScript = "demo.GameServer";
	private static string ClientScript = "demo.GameClient";
	
	private Server mServer;
	private Client mClient;
	
	public native ScriptManager getScriptManager();
	public native GameWindowFactory getWindowFactory();
	public native void setActive(boolean active);
	
	public boolean initialize()
	{
		FileSystem fs = FileSystem.getInstance();
		File file = fs.open("test.xml", 1);
		XmlDocument doc = new XmlDocument(file);
		
		return createServer() && createClient();
	}
   
	public void shutdown()
	{
		mClient.destroy();
		mServer.destroy();
	}
   
	public void run(real delta)
	{
		mServer.update(delta);
		
		mClient.update(delta);
		mClient.render(delta);
		
		if ( !mClient.isActive() )
		{
			setActive(false);
		}
	}
	
	private boolean createServer()
	{
		ClassLoader loader = ClassLoader.getInstance();
		try
		{
			Class serverclass = loader.loadClass(ServerScript);
		
			mServer = (Server) serverclass.newInstance();
			mServer.setScriptManager(getScriptManager().spawnChild());
			if ( !mServer.create() )
			{
				return false;
			}
		}
		catch ( Throwable e )
		{
			return false;
		}
		
		return mServer.listen(7000);
	}
	
	private boolean createClient()
	{
		ClassLoader loader = ClassLoader.getInstance();
		try
		{
			Class clientclass = loader.loadClass(ClientScript);
		
			mClient = (Client) clientclass.newInstance();
			mClient.setScriptManager(getScriptManager().spawnChild());
			mClient.setWindow(getWindowFactory().createWindow());
			if ( !mClient.create() )
			{
				return false;
			}
		}
		catch ( Throwable e )
		{
			return false;
		}
		
		return mClient.connect("localhost", 7000, "player");
	}
}

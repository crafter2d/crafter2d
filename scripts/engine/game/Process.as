
package engine.game;

use engine.core.*;
use engine.collections.*;
use engine.messages.*;
use engine.net.*;
use engine.streams.*;

// Class : Process
// Abstract base class for the client and server processes

abstract class Process
{
	protected BufferedStream mBufferedStream = new BufferedStream();
	protected NetStream mStream = new NetStream(mBufferedStream);
	
	private MessageMap mMessageMap     = new MessageMap();
	private ArrayList<Player> mPlayers = new ArrayList<Player>();
	
	private World mWorld;
	
	public static int CONNECT_ALLOW = 0;
	public static int CONNECT_DENY_STARTED = 1;
	
	/// called by the native process when it is created
	private boolean onCreated()
	{		
		boolean success = create();
		if ( success )
		{	
			// Register the messages
			registerMessages(mMessageMap);
		}
		return success;
	}
	
	public abstract boolean create();
	
	public void onScriptEvent(NetStream stream)
	{
		int msgid = stream.readInt();
		Message message = mMessageMap.getMessage(msgid);
		message.read(stream);
			
		onMessageReceived(message);
	}
	
	// - Get/set
	
	public void addPlayer(Player player)
	{
		mPlayers.add(player);
	}
	
	public ArrayList<Player> getPlayers()
	{
		return mPlayers;
	}
	
	public Player getPlayer(int id)
	{
		return mPlayers.get(id);
	}
	
	public World getWorld()
	{
		return mWorld;
	}
	
	public native void setActionMap(ActionMap map);
	
	/// does not call the native function
	protected void setInternalWorld(World world)
	{
		mWorld = world;
	}
	
	public boolean createWorld(string filename)
	{
		mWorld = loadWorld(filename);
		return mWorld != null;
	}
	
	// - Overloadables
	
	protected void registerMessages(MessageMap messagemap)
	{
	}
	
	protected void onMessageReceived(Message message)
	{
	}
	
	// - Memory leak detection
	
	public native void swapLeakDetection();
	
	// - Natives
	
	public native ContentManager getContentManager();
	public native ScriptManager getScriptManager();
	
	private native World loadWorld(string filename);
}

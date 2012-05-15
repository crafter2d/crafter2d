
use engine.ui.*;
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
	
	public static int CONNECT_ALLOW = 0;
	public static int CONNECT_DENY_STARTED = 1;
	
	// natives
	
	private native boolean create(Object self);
	
	public native void destroy();
	public native ScriptManager getScriptManager();
	public native void setScriptManager(ScriptManager scriptmanager);
	public native Font getFont(string name, int size);
	public native Texture getTexture(string name);
	public native ContentManager getContentManager();
	public native World getWorld();
	public native void setWorld(World world);
	
	public boolean create()
	{		
		boolean success = create(this);
		if ( success )
		{	
			// Register the messages
			registerMessages(mMessageMap);
		}
		return success;
	}
	
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
	
	public Player getPlayers()
	{
		return mPlayers.get(0);
	}
	
	// - Overloadables
	
	protected void registerMessages(MessageMap messagemap)
	{
	}
	
	protected void onMessageReceived(Message message)
	{
	}
}

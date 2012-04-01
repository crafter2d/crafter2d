
use engine.ui.*;
use engine.core.*;
use engine.collections.*;
use engine.messages.*;

// Class : Process
// Abstract base class for the client and server processes

abstract class Process
{
	protected BitStream mStream = new BitStream();
	
	private MessageMap mMessageMap     = new MessageMap();
	private ArrayList<Player> mPlayers = new ArrayList<Player>();
	
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
	
	public void onScriptEvent(BitStream stream)
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

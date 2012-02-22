
use engine.ui.*;
use engine.core.*;
use engine.messages.*;

// Class : Process
// Abstract base class for the client and server processes

abstract class Process
{
	protected BitStream mStream = new BitStream();
	
	private MessageMap mMessageMap = new MessageMap();
	
	// natives
	
	private native boolean create(string name);
	private native void setObject(Object self);
	
	public native void setScriptManager(ScriptManager manager);
	public native Font getFont(string name, int size);
	public native Texture getTexture(string name);
	public native ContentManager getContentManager();
	public native World getWorld();
	public native void setWorld(World world);
	
	public boolean create()
	{		
		string classname = this.class.getName();
		boolean success = create(classname);
		if ( success )
		{
			// Let the process now this object so it can be used in
			// both the process space as in the game space.
			setObject(this);
			
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
	
	protected void registerMessages(MessageMap messagemap)
	{
	}
	
	protected void onMessageReceived(Message message)
	{
	}
}

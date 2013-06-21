
package engine.messages;

use engine.net.*;

class ActionMessage extends Message
{
	private int mAction;
	private int mActorId;
	
	public static int ID = 2;
	
	public ActionMessage()
	{
		super();
	}

	public ActionMessage(int actorid, int action)
	{
		super();
		
		mActorId = actorid;
		mAction = action;
	}
	
	// get/set
	
	public int getId()
	{
		return ID;
	}
	
	public int getActorId()
	{
		return mActorId;
	}
	
	public int getAction()
	{
		return mAction;
	}
	
	// streaming
	
	public void read(NetStream stream)
	{
		mActorId = stream.readInt();
		mAction = stream.readInt();
	}
	
	public void write(Netstream stream)
	{
		stream.writeInt(mActorId);
		stream.writeInt(mAction);
	}
}

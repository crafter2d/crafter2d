
package engine.messages;

use engine.net.*;

class ControllerMessage extends Message
{
	private int mControllerId;
	
	public static int ID = 1;
	
	public ControllerMessage()
	{
		super();
	}

	public ControllerMessage(int controllerid)
	{
		super();
		mControllerId = controllerid;
	}
	
	// get/set
	
	public int getId()
	{
		return ID;
	}
	
	public int getControllerId()
	{
		return mControllerId;
	}
	
	// streaming
	
	public void read(NetStream stream)
	{
		mControllerId = stream.readInt();
	}
	
	public void write(Netstream stream)
	{
		stream.writeInt(mControllerId);
	}
}

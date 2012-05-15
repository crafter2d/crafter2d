
use system.*;
use engine.messages.*;
use engine.net.*;

abstract class Server extends Process
{	
	public native Server();

	public native boolean listen(int port);
	public native void update(real delta);
	public native void sendScriptEvent(NetStream stream, int client);

	protected native boolean loadWorld(string filename, string name);
	
	public void sendMessage(Message message, int clientid)
	{
		mStream.clear();
		mStream.writeInt(message.getId());
		message.write(mStream);
		
		sendScriptEvent(mStream, clientid);
	}
}


package engine.messages;

use engine.net.*;

abstract class Message
{
	public abstract int getId();
	// streaming
	
	public abstract void read(NetStream stream);
	public abstract void write(Netstream stream);
}

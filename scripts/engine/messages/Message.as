
package engine.messages;

abstract class Message
{
	public abstract int getId();
	// streaming
	
	public abstract void read(BitStream stream);
	public abstract void write(Bitstream stream);
}

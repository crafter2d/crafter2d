
package engine.net;

use engine.streams.*;

final class NetStream
{
	public native NetStream(BufferedStream stream);
	
	public native int readInt();
	public native void writeInt(int value);
	
	public native void clear();
}

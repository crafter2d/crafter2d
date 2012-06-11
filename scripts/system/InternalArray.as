
package system;

/// This class should not be used directly, it is used by the virtual machine.

class InternalArray
{
	public final int length;
	
	public native void resize(int newsize);
}

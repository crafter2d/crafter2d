
package system;

/// This class should not be used directly, it is used by the virtual machine.

class InternalArray
{
	public native int size();
	
	public native void resize(int newsize);
}

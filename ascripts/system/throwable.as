
package System;

class Throwable
{
	private string mCause;
	private string mCallStack;
	
	public Throwable()
	{
		this("");
	}
	
	public Throwable(string cause)
	{
		super();
		
		//fillCallStack();
		
		mCause = cause;
	}
	
	public string getCause()
	{
		return mCause;
	}
	
	//private native void fillCallStack();
}

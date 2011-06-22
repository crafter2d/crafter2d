
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
		
		mCause = cause;
		mCallStack = fillCallStack();		
	}
	
	public string getCause()
	{
		return mCause;
	}
	
	public string getCallStack()
	{
		return mCallStack;
	}
	
	public void setCause(string cause)
	{
		mCause = cause;
	}
	
	private native string fillCallStack();
}


class Throwable
{
	private string mCause;
	private string mCallStack;
	
	public Throwable()
	{
		//this("");
	}
	
	public Throwable(string cause)
	{
		//fillCallStack();
		
		mCause = cause;
	}
	
	public string getCause()
	{
		return mCause;
	}
	
	//private native void fillCallStack();
}

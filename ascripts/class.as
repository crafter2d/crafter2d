
use function;
use nosuchfunctionexception;

class Class
{
	private string     mName;
	private Function[] mFunctions;
	
	public string getName()
	{
		return mName;
	}
	
	public Function getFunction(string name)
	{
		for ( int index = 0; index < mFunctions.length; index++ )
		{
			Function f = mFunctions[index];
			if ( f.getName() == name )
			{
				return f;
			}
		}
		
		throw new NoSuchFunctionException();
	}
	
	public Function[] getFunctions()
	{
		return mFunctions;
	}
	
	public Object newInstance()
	{
		return doNewInstance(this);
	}
	
	private native Object doNewInstance(Class c);
}

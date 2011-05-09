
use arraylist;
use Class;
use classnotfoundexception;

class ClassLoader
{
	private static ClassLoader mLoader = new ClassLoader();

	private ArrayList<Class> mClasses;
	
	private ClassLoader()
	{
		mClasses = new ArrayList<Class>();
	}
	
	private void classLoaded(Class c)
	{
		// called by the VM
		mClasses.add(c);
	}
	
	public static ClassLoader getInstance()
	{
		return mLoader;
	}
	
	public boolean isLoaded(string name)
	{
		foreach( Class c : mClasses )
		{
			if ( c.mName == name )
			{
				return true;
			}
		}
		return false;
	}
	
	public Class findClass(string name)
	{
		foreach( Class c : mClasses )
		{
			if ( c.mName == name )
			{
				return c;
			}
		}
		
		throw new ClassNotFoundException();
	}
	
	public void loadClass(string name)
	{
		if ( !doLoadClass(name) )
		{
			throw new ClassNotFoundException();
		}
	}
	
	private native boolean doLoadClass(string name);
}

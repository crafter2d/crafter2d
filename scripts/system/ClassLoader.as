
package system;

use engine.collections.*;

class ClassLoader
{
	private static ClassLoader mLoader = new ClassLoader();

	private ArrayList<Class> mClasses;
	
	private ClassLoader()
	{
		super();
		
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
	
	public Class loadClass(string name)
	{
		if ( !doLoadClass(name) )
		{
			throw new ClassNotFoundException();
		}
		return findClass(name);
	}
	
	private native boolean doLoadClass(string name);
}

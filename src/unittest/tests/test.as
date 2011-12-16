
// c++ unittest test class

use System.*;

class Test
{
	TestException te;
	
	public void run()
	{
		try
		{
			te.run(); // null pointer exception, not initialized
		}
		catch ( NullPointerException e )
		{
			System.console.println(e.getCallStack());
		}
	}
}
 
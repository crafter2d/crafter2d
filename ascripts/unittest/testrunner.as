
package UnitTest;

class TestRunner
{
	private static TestRunner mRunner = new TestRunner();
	
	public static TestRunner getInstance()
	{
		return mRunner;
	}
	
	private TestRunner()
	{
	}
	
	public void run(Class c)
	{
		System.console.println("Testing class " + c.getName());
		
		Object instance = c.newInstance();
		
		Function[] fs = c.getFunctions();
		
		runPretest(instance, fs);
		runTests(instance, fs);
	}
	
	private void runPretest(Object instance, Function[] functions)
	{
		for ( int index = 0; index < functions.length; index++ )
		{
			Function f = functions[index];
			if ( f.hasAnnotation("pretest") )
			{
				f.invoke(instance);
			}
		}
	}
	
	private void runTests(Object instance, Function[] functions)
	{
		for ( int index = 0; index < functions.length; index++ )
		{
			Function f = functions[index];
			if ( f.hasAnnotation("test") )
			{
				System.console.println("Testing " + f.getName() + "\n");
				
				f.invoke(instance);
			}
		}
	}
	
	private void runTest(Object instance, Function test)
	{
		try
		{
			test.invoke(instance);
		}
		/*catch ( NoSuchFunctionException e )
		{
			System.console.println("Could not find function " + test.getName());
		}
		*/
		catch ( Throwable e )
		{
		}
	}
}

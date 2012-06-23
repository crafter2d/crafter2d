
package UnitTest;

use engine.collections.*;
use system.*;

// TestRunner
// A simple unit test framework to run unit tests.
//
// Usage:
// Register one or more classes to it using the addClass method. When
// all classes are registered, call the run() method to start running
// the tests.
//
// The methods that should be executed be marked with the @test annotation.
// Each class can also have one or more @pretest functions, which run just
// before the actual tests.

class TestRunner
{
	private static TestRunner mRunner = new TestRunner();
	
	public static TestRunner getInstance()
	{
      return mRunner;
	}
	
	private ArrayList<Class> mClasses = new ArrayList<Class>();
	private int mTests                = 0;
	private int mFailures             = 0;
	
	private TestRunner()
	{
		super();
	}
	
	public void addClass(Class c)
	{
		mClasses.add(c);
	}
	
	public void run(Class c)
	{
		addClass(c);
		run();
	}
	
	public void run()
	{
		System.console.println("\nRunning tests");
		
		foreach ( Class c : mClasses )
		{
			Object instance = c.newInstance();
			Function[] fs = c.getFunctions();
			
			runPretest(instance, fs);
			runTests(instance, fs);
		}
		
		System.console.println("Test results");
		System.console.println("Nr test: " + mTests);
		System.console.println("Failures: " + mFailures);
	}
	
	private void runPretest(Object instance, Function[] functions)
	{
		foreach ( Function f : functions )
		{
			if ( f.hasAnnotation("pretest") )
			{
				f.invoke(instance);
			}
		}
	}
	
	private void runTests(Object instance, Function[] functions)
	{
		foreach ( Function f : functions )
		{
			if ( f.hasAnnotation("test") )
			{
				mTests++;
				
				try
				{
					f.invoke(instance);
				}
				catch ( AssertionError ae )
				{
					System.console.println("Test " + f.getName() + " failed.");
					mFailures++;
				}
				catch ( Throwable t )
				{
					System.console.println("Caught unhandled exception during executing test " + f.getName());
					mFailures++;
				}				
			}
		}
	}
}

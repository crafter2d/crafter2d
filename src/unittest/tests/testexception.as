
// c++ unit testing for exceptions

class TestException
{
	@test
	public void testNullPointer()
	{
		try
		{
			TestException te;
			te.run(); // null pointer exception, not initialized
		}
		catch ( NullPointerException e )
		{
			System.console.println(e.getCallStack());
		}
	}
	
	private void run()
	{
	}
}


// c++ unit testing for exceptions

class TestException
{
	@test
	public void testNullPointer()
	{
		boolean handled = false;
		try
		{
			TestException te;
			te.run(); // null pointer exception, not initialized
		}
		catch ( NullPointerException e )
		{
			handled = true;
		}
		
		assert handled;
	}
	
	@test
	public void testFinallyWithoutException()
	{
		boolean reached = false;
		try
		{
			TestException te = new TestException();
			te.run();
		}
		catch ( NullPointerException e )
		{
		}
		finally
		{
			reached = true;
		}
		
		assert reached;
	}
	
	@test
	public void testFinallyWithException()
	{
		boolean handled = false;
		boolean reached = false;
		try
		{
			TestException te = new TestException();
			te.throwException();
		}
		catch ( NullPointerException e )
		{
			handled = true;
		}
		finally
		{
			reached = true;
		}
		
		assert handled;
		assert reached;
	}
	
	private void run()
	{
	}
	
	private void throwException()
	{
		throw new NullPointerException();
	}
}


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
		
		// assert handled;
	}
	
	private void run()
	{
	}
}

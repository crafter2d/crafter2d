
// c++ unit testing for switch statement

class TestSwitch
{
	@test
	public void testDefault()
	{
		boolean found = false;
		boolean cases = false;
		int value = 5;
		
		switch ( value )
		{
			case 1:
				cases = true;
				break;
			case 10:
				cases = true;
				break;
			default:
				found = true;
				break;
		}
		
		assert !cases;
		assert found;
	}
	
	
	@test
	public void testFallThrough()
	{
		boolean found = false;
		boolean reached = false;
		
		int value = 5;
		switch ( value )
		{
			case 5:
				found = true;
			case 6:
				reached = true;
				break;
			default:
				found = false;
				break;
		}
		
		assert found;
		assert reached;
	}
	
	@test
	public void testInt()
	{
		boolean found = false;
		int value = 5;
		switch ( value )
		{
			case 5:
				found = true;
				break;
			default:
				found = false;
				break;
		}
		assert found;
	}
	
	@test
	public void testReal()
	{
		boolean found = false;
		real value = 5.0;
		switch ( value )
		{
			case 1.2345:
				found = false;
				break;
			case 5.0:
				found = true;
				break;
			default:
				found = false;
				break;
		}
		assert found;
	}
	
	@test
	public void testChar()
	{
		boolean found = false;
		char value = 'c';
		switch ( value )
		{
			case 'a':
				found = false;
				break;
			case 'c':
				found = true;
				break;
			default:
				found = false;
				break;
		}
		
		assert found;
	}
	
	@test
	public void testString()
	{
		boolean found = false;
		string value = "value";
		switch ( value )
		{
			case "anothervalue":
				found = false;
				break;
			case "value":
				found = true;
				break;
			default:
				found = false;
				break;
		}
		
		assert found;
	}
}
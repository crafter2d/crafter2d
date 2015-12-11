
// c++ unit testing for loops

class TestLoops
{
	@test
	public void testSimpleForLoop()
	{
		int result = 0;
		for ( int i = 1; i < 5; i++ )
		{
			result = result + i;
		}
		
		assert result == 10;
	}
	
	@test
	public void testForeachOnArray()
	{
		int[] intarray = new int[2];
		intarray[0] = 0;
		intarray[1] = 1;
		
		int index = 0;
		foreach ( int i : intarray )
		{
			assert i == index;
			
			index++;
		}
	}
	
	@test
	public void testWhileLoop()
	{
		int result = 0;
		int index = 1;
		while ( index < 5 )
		{
			result = result + index;
			
			index++;
		}
		
		assert result == 10;
	}
	
	@test
	public void testDoLoop()
	{
		int result = 0;
		int index = 1;
		do
		{
			result = result + index;
			
			index++;
		}
		while ( index < 5 )
		
		assert result == 10;
	}
}

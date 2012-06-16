
// c++ unit testing map

use engine.collections.*;
use system.*;

class TestArray
{
	private void validateArray(int[] array)
	{
		for ( int index = 1; index <= array.length; index++ )
		{
			assert array[index-1] == index;
		}
	}

	@test
	public void testCreateArray()
	{
		int[] array = new int[1];
		
		assert array != null;
	}
	
	@test
	public void testFillArray()
	{
		int [] array = new int[1];
		array[0] = 5;
		
		assert array[0] == 5;
	}
	
	@test
	public void testArrayIndexOutOfBoundsExceptionThrown()
	{
		int[] array = new int[1];
		
		boolean wrongexc = false;
		boolean thrown = false;
		
		try
		{
			array[5] = 5;
		}
		catch ( ArrayIndexOutOfBoundsException e )
		{
			thrown = true;
		}
		catch ( Throwable t )
		{
			wrongexc = true;
		}
		
		assert thrown;
		assert !wrongexc;
	}
	
	@test
	public void testArrayInitializer()
	{
		int[] array = { 1, 2 };
		validateArray(array);
	}
	
	@test
	public void testArrayCopy()
	{
		int[] array = { 1, 2 };
		int[] to = new int[2];
		
		Arrays.copy<int>(array, to);
		validateArray(array);
	}
	
	@test
	public void testArrayEquals()
	{
		int[] one = { 1, 2 };
		int[] two = { 1, 2 };
		int[] three = { 3, 4 };
		
		assert Arrays.equals<int>(one, two);
		assert !Arrays.equals<int>(one, three);
	}
}

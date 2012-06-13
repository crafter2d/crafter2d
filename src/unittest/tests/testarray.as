
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
}

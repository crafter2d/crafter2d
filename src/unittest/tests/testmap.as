
// c++ unit testing map

use engine.collections.*;
use system.*;

class TestMap
{
	@test
	public void testInsertAndRetrieval()
	{
		Map< int, GenericClass<int> > map = new Map< int, GenericClass<int> >();
		
		GenericClass<int> gc = new GenericClass<int>();
		gc.set(5);
		
		map.insert(5, gc);
		gc = map.get(5);
		
		assert gc != null;
		assert gc.get() == 5;
	}
	
	@test
	public void testRetrieveUnavailableItem()
	{
		Map< int, GenericClass<int> > map = new Map< int, GenericClass<int> >();
		
		GenericClass<int> gc = map.get(10);
		
		assert gc == null;
	}
}


// c++ unit testing for generics

class TestGenerics
{
	@test
	public void testGenericSingle()
	{
		GenericClass<int> gen = new GenericClass<int>();
		gen.set(5);
		
		assert gen.get() == 5;
	}
}

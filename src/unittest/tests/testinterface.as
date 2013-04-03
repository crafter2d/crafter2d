
// c++ unit testing for interfaces

class TestInterface implements InterfaceClass
{
	public int getValue()
	{
		return 100;
	}

	@test
	public void testInterfaceCall()
	{
		TestInterface test = new TestInterface();
		int value = test.getValue();
		assert value == 100;
	}
	
	@test
	public void testInterfaceAssign()
	{
		InterfaceClass test = new TestInterface();
		int value = test.getValue();
		assert value == 100;
	}
}


// c++ unit testing for interfaces

class TestInterface implements InterfaceClass
{
	int mValue = 100;
	
	public int getValue()
	{
		return mValue;
	}
	
	public void setValue(int value)
	{
		mValue = value;
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
	
	@test
	public void testInterfaceArgumentCall()
	{
		InterfaceClass test = new TestInterface();
		test.setValue(256);
		
		assert test.getValue() == 256;
	}
}


// c++ unit testing string

use system.*;

class TestNative
{
	@test
	public void testPureNativeConstructorAndIntReturn()
	{
		NativeClass nativeClass = new NativeClass();
		assert nativeClass != null;
		assert nativeClass->getIntValue() == 5;
	}
	
	@test
	public void testNativeStringReturn()
	{
		NativeClass nativeClass = new NativeClass();
		assert nativeClass.getStringValue() == "test";
	}
	
	@test
	public void testArgumentsPassedToNative()
	{
		NativeClass nativeClass = new NativeClass();
		assert nativeClass.add(5, 6) == 11;
	}
	
	@test
	public void testArgumentsPassedToPureNative()
	{
		NativeClass nativeClass = new NativeClass();
		assert nativeClass.mul(5, 6) == 30;
	}
	
	@test
	public void testArgumentsNotPassedToNative()
	{
		NativeClass nativeClass = new NativeClass();
		assert nativeClass.sub(6, 5) == 1;
	}
}

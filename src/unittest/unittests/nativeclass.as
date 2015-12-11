
// class used for generics testing

native class NativeClass
{
	native NativeClass();
	
	public native int getIntValue();
	
	public string getStringValue()
	{
		return native;
	}
	
	public native int add(int a, int b)
	{
		return native(a, b);
	}
	
	public native int sub(int a, int b)
	{
		return native;
	}
	
	public native int mul(int a, int b);
}

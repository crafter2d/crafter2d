
class BaseClass
{
	public static int mStaticMember = 0;
	protected int mMember = 0;
	
	public void setMemberBase(int value)
	{
		mMember = value;
	}
	
	public int getMemberBase()
	{
		return mMember;
	}
	
	public string getBaseValue()
	{
		return "value";
	}
}
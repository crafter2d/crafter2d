
class DerivedClass extends BaseClass
{
	public void setMemberDerived(int value)
	{
		mMember = value;
	}
	
	public string getBaseValue()
	{
		return super.getBaseValue();
	}
	
	public int getStaticProtectedMember()
	{
		return BaseClass.mStaticProtectedMember;
	}
}

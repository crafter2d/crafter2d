
use engine.collections.*;
use engine.messages.*;

class MessageMap
{
	private Map<int, Class> mMessages = new Map<int, Class>();
	
	public void register(int messageid, Class msgclass)
	{
		mMessages.insert(messageid, msgclass);
	}
	
	public Message getMessage(int id)
	{
		Class msgclass = mMessages.get(id);
		if ( msgclass != null )
		{
			return (Message)msgclass.newInstance();
		}
		
		return null;
	}
}


use player;

class Server
{
	private boolean mStarted = false;
	
	public native Server();
	public native boolean create();
	public native void listen(int port);
	public native void update(real delta);

	private native boolean loadWorld(string filename, string name);
	
	public int onClientConnecting()
	{
		return 0;
	}
	
	public void onClientConnect(Player player)
	{
		start();
	}
	
	private void start()
	{
		mStarted = true;
		
		if ( !loadWorld("../worlds/map1.jwl", "World") )
		{
		}
	}
}

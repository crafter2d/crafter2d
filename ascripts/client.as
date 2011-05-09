
class Client
{
	public native Client();
	public native boolean create();
	public native void connect(string host, int port, string name);
	public native void update(real delta);
	public native void render(real delta);
	
	public void onConnected()
	{
	}
	
	public void onWorldChanged()
	{
	}
}

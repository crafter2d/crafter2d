
use client;
use server;

class Game
{
	private Server mServer;
	private Client mClient;
	
	public void initialize()
	{
		mServer = new Server();
		mServer.create();
		mServer.listen(7000);
		
		mClient = new Client();
		mClient.create();
		mClient.connect("localhost", 7000, "player");
	}
   
	public void shutdown()
	{
	}
   
	public void run(real delta)
	{
		mServer.update(delta);
		
		mClient.update(delta);
		mClient.render(delta);
	}
}


package engine.game;

use system.*;
use engine.messages.*;
use engine.net.*;

/// This class is the actual server. It is responsible for the game logic and is leading
/// for the actual object states.

abstract class Server extends Process
{	
	/// called by the engine when a player tries to join, can be overloaded to
	/// deny new players to join
	/// return  0 when the player is allowed to join
	///			<> 1 to deny a player to join the game, the connection will be closed to this player
	public int onClientConnecting()
	{
		return Process.CONNECT_ALLOW;
	}

	/// called by the engine when a player has successfully connected to the server
	/// player : the player that just joined
	public void onClientConnect(Player player)
	{
		addPlayer(player);
	}
	
	/// called by the engine when the server shuts down
	public void onShutdown()
	{
	}
	
	/// sends a message to the given client.
	/// message : the message that will be transmitted
	/// clientid: the id of the client
	public void sendMessage(int clientid, Message message)
	{
		mStream.clear();
		mStream.writeInt(message.getId());
		message.write(mStream);
		
		sendScriptEvent(clientid, mStream);
	}
	
	public native boolean listen(int port);
	public native void sendScriptEvent(int clientid, NetStream stream);
}


package engine.game;

use engine.collections.*;

class AIController extends Controller
{
	private Process mProcess;
	
	public native AIController(Process process)
	{
		super();
		
		mProcess = process;
	}
	
	public void updateAI(Actor actor)
	{
	/*
		ArrayList<Player> players = mProcess.getPlayers();
		foreach ( Player player : players )
		{
			actor.updateAI(player.getController());
		}
		*/
		
		Player player = mProcess.getPlayers();
		actor.updateAI(player.getController());
	}
}
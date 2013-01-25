
package engine.game;

use ActionMap;

class InputController extends Controller
{
	private ActionMap mActionMap = null;
	
	public native InputController();
	
	// - Get/set
	
	public ActionMap getActionMap()
	{
		return mActionMap;
	}
	
	public void setActionMap(ActionMap map)
	{
		mActionMap = map;
		native;
	}
}

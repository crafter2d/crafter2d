
package engine.game;

use ActionMap;

class InputController extends Controller
{
	private ActionMap mActionMap = null;
	
	// - Get/set
	
	public ActionMap getActionMap()
	{
		return mActionMap;
	}
	
	public void setActionMap(ActionMap map)
	{
		mActionMap = map;
		native_setActionMap(map);
	}
	
	public native InputController();
	private native void native_setActionMap(ActionMap actionmap);
}

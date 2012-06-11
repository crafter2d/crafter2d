
package engine.ui;

use engine.core.*;
use engine.input.*;

class EditorMouseListener extends MouseListener
{
	private Editor mEditor;
	
	public EditorMouseListener(Editor editor)
	{
		mEditor = editor;
	}
	
	public void onMouseEvent(MouseEvent event)
	{
		if ( event.isLeftButton() && event.isDown() )
		{
			InputFocusManager.setInputFocus(mEditor);
		}
	}
}

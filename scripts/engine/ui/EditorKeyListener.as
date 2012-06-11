
package engine.ui;

use engine.input.*;

class EditorKeyListener extends KeyListener
{
	private Editor mEditor;
	
	public EditorKeyListener(Editor editor)
	{
		mEditor = editor;
	}
	
	// - overloads
	
	public void onKeyEvent(KeyEvent event)
	{
		if ( event.isPressed() )
		{
			mEditor.insert(event.getKey());
		}
	}
}

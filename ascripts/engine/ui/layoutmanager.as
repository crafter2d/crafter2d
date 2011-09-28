
package engine.ui;

/** 
 * LayoutManager interface
 */

interface LayoutManager
{
	void addWidget(Widget widget, Object constraint);
	
	void layout(Container parent);
}

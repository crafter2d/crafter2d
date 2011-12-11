
package engine.ui;

use engine.core.*;
use engine.shapes.*;

/**
 * BorderLayout
 * Positions the children based on position:
 *  - one center
 *  - one each border
 */

class BorderLayout implements LayoutManager
{
	private Widget mCenter = null;
	private Widget mLeft   = null;
	private Widget mRight  = null;
	private Widget mTop    = null;
	private Widget mBottom = null;

	void addWidget(Widget widget, Object constraint)
	{
		if ( constraint instanceof BorderLayoutSide )
		{
			BorderLayoutSide side = (BorderLayoutSide)constraint;
			
			switch ( side.getSide() )
			{
			case BorderLayoutSide.LEFT:
				mLeft = widget;
				break;
			case BorderLayoutSide.RIGHT:
				mRight = widget;
				break;
			case BorderLayoutSide.TOP:
				mTop = widget;
				break;
			case BorderLayoutSide.BOTTOM:
				mBottom = widget;
				break;
			case BorderLayoutSide.CENTER:
				mCenter = widget;
				break;
			}
		}
		else
		{
			mCenter = widget;
		}
	}

	void layout(Container parent)
	{
		Rectangle parentbounds = parent.getBounds();
		Rectangle bounds = new Rectangle(0, 0, parentbounds.width, parentbounds.height);
		Border border = parent.getBorder();
		if ( border != null )
		{
			Margins margins = border.getMargins();
			
			// resize rectangle to client area
			bounds.x = margins.left;
			bounds.y = margins.top;
			bounds.width = bounds.width - margins.left - margins.right;
			bounds.height = bounds.height - margins.top - margins.bottom;
		}
	
		if ( mTop != null )
		{
			Size size = mTop.getPreferredSize();
			mTop.setBounds(bounds.x, bounds.y, bounds.width, size.height);
			bounds.y = size.height;
		}
		if ( mBottom != null )
		{
			Size size = mBottom.getPreferredSize();
			mBottom.setBounds(bounds.x, bounds.getBottom() - size.height, bounds.width, size.height);
			bounds.height = bounds.height - size.height;
		}
		if ( mLeft != null )
		{
			Size size = mLeft.getPreferredSize();
			mLeft.setBounds(bounds.x, bounds.y, size.width, bounds.height);
			bounds.x = bounds.x + size.width;
		}
		if ( mRight != null )
		{
			Size size = mRight.getPreferredSize();
			mRight.setBounds(bounds.x + bounds.width - size.width, bounds.y, size.width, bounds.height);
			bounds.width = bounds.width - size.width;
		}
		if ( mCenter != null )
		{
			// the center component gets the remaining space
			mCenter.setBounds(bounds);
		}
	}
}

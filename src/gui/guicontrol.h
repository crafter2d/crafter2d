/***************************************************************************
 *   Copyright (C) 2005 by Jeroen Broekhuizen                              *
 *   jeroen@nwnx.homedns.org                                               *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#ifndef _GUICONTROL_H_
#define _GUICONTROL_H_

#include <map>
#include "guiwnd.h"
#include "guigraphics.h"

enum ControlCallback {
   CB_SETFOCUS       = 1,
   CB_KILLFOCUS      = 2,

   LB_SELCHANGE      = 3,

   CTRL_LBUTTONUP    = 400
};

/*! @author Jeroen Broekhuizen

@section guiControl_intro Introduction
GuiControl is the base class for all controls in JEngine SSE. Controls, also known
as widgets, are the interface tools with which the user interacts. It is therefore
important that all controls behave the same in the same manner. This class takes
care of the basic functionality.

@section guiControl_designer Registering with Designer
To allow easy creation of the control in the Designer, it should be registered to it.
If a control is not registered, a user can not create that control in the designer.
Registering can be done with two easy to use macro's as discussed in this section.

In the class definition the macro DESIGNER_REGISTRATION should be added somewhere
in the public part of the class as show in the following code sample. The macro
expects only one parameter: the name of the class.
@code
class GuiEditBox {
public:
   DESIGNER_REGISTRATION(GuiEditBox)
   GuiEditBox();
   ...
}
@endcode

In the implementation section of the class the actual registering takes place. To do so
use the REGISTER_DESIGNER macro. It requires four parameters. The first is again the class
name. The second is a unique id for this control (see the GuiControlIds enum in guidefines.h)
which may never change! If it does change, existing user interfaces using that control
will not load any more. The third parameter is the name of the control as shown in the
Designer. This can be any string. The fourth and fifth parameters are respectively the width
and height of the control when it is instanciated in the designer. The last parameter
are the default flags of the control when it is created. The sample below registers the
edit box as EditBox to the designer. And at creation it will have the size of 40 pixels
width and 15 height and some flags.
@code
REGISTER_DESIGNER(GuiEditBox, GuiEditBoxId, "Editbox", 40, 15, 392)
GuiEditBox::GuiEditBox() {
...
@endcode
*/
class GuiControl: public GuiWnd
{
public:
   typedef std::map<int, std::string> CallbackMap;

                     GuiControl();
   virtual           ~GuiControl();

	virtual void      destroy();

           void      setCallback(int type, const char* fnc);

   virtual int       getTypeId() const;

 // Input interface
   virtual int       onLButtonUp(const GuiPoint& point, int flag);

 // layout
   virtual float     getPreferredWidth() const;
   virtual float     getPreferredHeight() const;

   virtual bool      growHorizontally() const;
   virtual bool      growVertically() const;

protected:
   virtual void      onCreate(const GuiRect& rect, const char* caption, GuiStyle style, GuiWnd* parent);
   virtual void      onRender(Uint32 tick, const GuiGraphics& graphics);
   virtual void      paint(Uint32 tick, const GuiGraphics& graphics);

   GuiColor          backgroundColor() const;
   void              callback(int type);

private:
   CallbackMap callbacks;
   GuiColor    _background;
};

#ifdef JENGINE_INLINE
#  include "guicontrol.inl"
#endif

#endif

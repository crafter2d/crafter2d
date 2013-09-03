/***************************************************************************
 *   Copyright (C) 2009 by Jeroen Broekhuizen                              *
 *   jengine.sse@live.nl                                                   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU Library General Public License as       *
 *   published by the Free Software Foundation; either version 2 of the    *
 *   License, or (at your option) any later version.                       *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU Library General Public     *
 *   License along with this program; if not, write to the                 *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#ifndef MOUSE_LISTENER_H
#define MOUSE_LISTENER_H

namespace Input
{
   class MouseEvent;

   class MouseListener
   {
   public:
      MouseListener();
      virtual ~MouseListener();

      /// \fn MouseListener::onMouseButton(const MouseEvent& event)
      /// Called when a button is pressed or released
      virtual void onMouseButton(const MouseEvent& event);

      /// \fn MouseListener::onMouseClick(const MouseEvent& event)
      /// Called when a click with a button is detected
      virtual void onMouseClick(const MouseEvent& event);

      /// \fn onMouseMotion(const MouseEvent& event)
      /// Called when the mouse moves
      virtual void onMouseMotion(const MouseEvent& event);

      /// \fn onMouseContext(const MouseEvent& event)
      /// Called when a right click was detected. Can be used to popup a context menu. 
      virtual void onMouseContext(const MouseEvent& event);

      // \fn onMouseEntered(const MouseEvent& event);
      // Called when the mouse enters the window
      virtual void onMouseEntered(const MouseEvent& event);

      // \fn onMouseExited(const MouseEvent& event);
      // Called when the mouse leaves the window
      virtual void onMouseExited(const MouseEvent& event);
   };
}

#endif

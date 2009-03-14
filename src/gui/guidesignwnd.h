/***************************************************************************
 *   Copyright (C) 2006 by Jeroen Broekhuizen                              *
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
#ifndef GUI_DESIGNWND_H
#define GUI_DESIGNWND_H

#include <list>
#include <vector>
#include "guidialog.h"

class GuiDesignDecorator;
class GuiPopupDecorator;
class GuiDesignSelection;

/**
@author Jeroen Broekhuizen
*/
class GuiDesignWnd : public GuiDialog
{
public:
   typedef std::vector<std::string> Functions;
   typedef enum { alignLeft, alignRight, alignTop, alignBottom } GuiDesignAlign;
   typedef enum { sizeWidth, sizeHeight } GuiDesignSize;

            GuiDesignWnd();
   virtual ~GuiDesignWnd();

 // Operations
   void  deleteSelected();
   void  moveSelected(const GuiPoint& rel);
   void  resizeSelected(const GuiPoint& rel, int borders);
   int   selectionSize() const;
   void  unselectAll();

   void  save();
   void  load(const std::string& file);
   void  close();

   void  align(GuiWnd& main, GuiDesignAlign how);
   void  sameSize(GuiWnd& main, GuiDesignSize how);

 // Overloads
   virtual int       onLButtonDown(const GuiPoint& point, int flags);
   virtual int       onLButtonUp(const GuiPoint& point, int flag);
   virtual void      onMouseMove(const GuiPoint& point, const GuiPoint& rel, int flag);
   virtual void      onSetFocus(GuiWnd* oldCtrl);
   virtual void      onKillFocus(GuiWnd* newCtrl);
   virtual bool      onContextMenu(const GuiPoint& point);
   virtual void      onCommand(int id);
   virtual void      sendMessage(GuiId id, GuiEvent event, int param1);

protected:
 // Overloaded
   virtual void      onCreate(const GuiRect& rect, const char* caption, GuiStyle style, GuiWnd* parent);
   virtual void      onResize(int width, int height);

   void  onMenuLostFocus();

private:
   void  createMenu();
   void  doSave();

   GuiDesignSelection*  _pselectionctrl;
   GuiPopupDecorator*   _popupMenu;
   Functions            _functions;
   std::string          _filename;
   bool                 _selected;
};

#endif

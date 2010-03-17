/***************************************************************************
 *   Copyright (C) 2004 by Jeroen Broekhuizen                              *
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

/* This file contains the prototypes for the toLua program to bind these
 * classes to Lua. GUI part of the classes.
 */

$#include "gui/guicontrol.h"
$#include "gui/guiheadercontrol.h"
$#include "gui/guibutton.h"
$#include "gui/guiimagebutton.h"
$#include "gui/guicanvas.h"
$#include "gui/guidialog/guidialog.h"
$#include "gui/guigamewnd.h"
$#include "gui/guiedit.h"
$#include "gui/guistatic.h"
$#include "gui/guilistbox.h"
$#include "gui/guicheckbox.h"
$#include "gui/guicombobox.h"
$#include "gui/guiscriptwnd.h"
$#include "gui/guiimage.h"
$#include "gui/guifocus.h"
$#include "gui/guimanager.h"
$#include "gui/guimenu.h"
$#include "gui/guieventhandlerdefinition.h"
$#include "gui/guieventhandlerdefinitions.h"
$#include "gui/guifilelistbox.h"
$#include "gui/guitreebox.h"
$#include <SDL/SDL.h>

int SDL_EnableKeyRepeat(int delay, int interval);

enum GuiStyles {
   GUI_TABSTOP       = 1,
   GUI_ACTIVE        = 2,
   GUI_PRESSED       = 4,
   GUI_VISIBLE       = 8,
   GUI_NOTIFYPARENT  = 16,
   GUI_NODESTROY     = 32,
   GUI_TOPMOST       = 64,
   GUI_BORDER        = 128,
   GUI_BACKGROUND    = 256
};

enum ControlCallback {
   CB_SETFOCUS       = 1,
   CB_KILLFOCUS      = 2
};

class GuiFocus
{
   bool     hasFocus() const;
   GuiWnd&  getFocus();
};

class GuiPoint
{
   GuiPoint();
   GuiPoint(int _x, int _y);
   GuiPoint(const GuiPoint& other);

   int x;
   int y;
};

class GuiColor
{
   GuiColor();
   GuiColor(int _r, int _g, int _b, int _a=255);

   float r;
   float g;
   float b;
};

class GuiRect
{
   GuiRect();
   GuiRect(float l, float r, float t, float b);

   void set(float l, float r, float t, float b);

   bool pointInRect(const GuiPoint& point);

   float left() const;
   float right() const;
   float top() const;
   float bottom() const;
};

class GuiEventHandlerDefinition
{
   const std::string& getName() const;

   const std::string& getCode() const;
   void               setCode(const std::string& code);
};

class GuiEventHandlerDefinitions
{
   GuiEventHandlerDefinition* getDefinition(int index);
   int size() const;
};

class GuiWnd
{
   void create(int id, const GuiRect& rect, const char* caption = "", int style=0, GuiWnd* parent=NULL);
   void addChild(GuiWnd* wnd);

   int            getId();
   const GuiRect& getWindowRect();
   void           moveWindow (int x, int y);

   void           setWindowPos(int x, int y);
   void           setVisible(bool vis=true);
   void           setCaption(const char* caption);
   void           setBackgroundImage(const char* file);
   void           setFocus();
   void           center();

   void           clientToWindow(GuiRect& rect) const;
   void           windowToClient(GuiRect& rect) const;
   void           windowToClient(GuiPoint& point);

   GuiWnd*        getParent() const;
   GuiWnd*        getItemById (int id);
   GuiWnd*        findChildByName(const std::string& name);
   std::string&   getCaption();

   GuiEventHandlerDefinitions& getEventHandlerDefinitions();
};

class GuiWindow: public GuiWnd
{
   GuiWindow();

   void setActive(bool active);
};

class GuiScriptWnd: public GuiWindow
{
   GuiScriptWnd();
   ~GuiScriptWnd();

   void setMouseMoveFnc(const char* fnc);
   void setLButtonDownFnc(const char* fnc);
   void setKeyDownFnc(const char* fnc);
};

class GuiGameWnd: public GuiScriptWnd
{
   GuiGameWnd();

   void pushMap(KeyMap* map);
   void popMap();
};

class GuiControl: public GuiWnd
{
   GuiControl();
   ~GuiControl();

   void setCallback(int type, const char* fnc);
};

class GuiDialog: public GuiWnd
{
   GuiDialog();

   int   doModal();
   void  endModal(int retValue);
};

class GuiStatic: public GuiControl
{
   GuiStatic();
   GuiStatic(int id, GuiRect& rect, const char* caption = "", int style=0, GuiWnd* parent=NULL);
};

class GuiButton: public GuiControl
{
   GuiButton();

   void setCommand (const char* cmd);
};

class GuiImageButton: public GuiButton
{
   GuiImageButton();

   void setImage(const char* image);
   void setPressedImage(const std::string& file);

   void setTextureCoordinates(const GuiRect& rect);
   void setOffset(const Vector& ofs);
};

class GuiCheckBox: public GuiControl
{
   GuiCheckBox();

   bool isChecked() const;
};

class GuiImage: public GuiWnd
{
   GuiImage();
   void setImage(const char* image);
   void setOffset(int x, int y);
};

class GuiMovie: public GuiImage
{
   GuiMovie();
   void setMovie(const char* movie);
};

class GuiEditBox: public GuiControl
{
   GuiEditBox();
};

class GuiHeaderCtrl: public GuiControl
{
   GuiHeaderCtrl();

   bool create (int id, int style, GuiWnd* parent);
   void addColumn(int width);
   void remove(int index);
};

class GuiListBox: public GuiControl
{
   GuiListBox();

   void removeAll();
   void removeAt(int index);
   int addString(const char* str);

   GuiHeaderCtrl& header();
   const char* getItemText(int index);
   int getItemData(int index);
   int getItemCount();
   int getCurSel();

   void setCurSel(int index);
   void setItemText(int index, const char* text, int subitem=0);
   void setItemData(int index, int data);

   void setEditableColumn(int column);

   void scrollTo(int idx);
};

class GuiFileListBox : public GuiListBox
{
   GuiFileListBox();
};

class GuiComboBox: public GuiControl
{
   GuiComboBox();

   void setCallback(const char* function);
   void setCurSel(int index);
   void setItemText(int index, const char* text);
   void setItemData(int index, int data);

   int getCurSel();
   const char* getItemText(int index);
   int getItemData(int index);

   int addString(const char* str);
};

class GuiColorComboBox: public GuiComboBox
{
   GuiColorComboBox();
   void addColor(GuiColor color);
};

class GuiTreeBox: public GuiControl
{
   GuiTreeBox();
};

class GuiMenu : public GuiControl
{
   void addItem(int id, const std::string& name);
};

class GuiCanvas: public GuiWnd
{
   enum GuiStandardColor {
      GuiWindowColor    = 0,
      GuiBorderColor    = 1,
      GuiHeaderColor    = 2,
      MaxGuiColors      = 3
   };

   static bool    isShift();
   static bool    isAlt();
   static bool    isCtrl();

   void quit();

   void doModal(GuiWnd* wnd);
   void endModal(int retValue);
   void pushWindow(GuiWnd* wnd);
   void popWindow(GuiWnd* wnd);
   void popWindow();

   void changeDefaultColor(int color, const GuiColor& newColor);

   bool    isActiveWnd(GuiWnd& window);
   GuiWnd& getActiveWnd();
};

class GuiManager
{
   GuiDialog* loadFromXML(const std::string& name);
   GuiMenu*   loadMenuFromXML(const std::string& name);

   void showPopup(GuiWnd& owner, const GuiPoint& point, const std::string& name);
};

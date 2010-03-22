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
#ifndef GUIDEFINES_H_
#define GUIDEFINES_H_

typedef unsigned int    GuiId;
typedef unsigned long   GuiStyle;

enum GuiStyles {
   GUI_TABSTOP       = 1,
   GUI_ACTIVE        = 2,
   GUI_PRESSED       = 4,
   GUI_VISIBLE       = 8,
   GUI_NOTIFYPARENT  = 16,
   GUI_NODESTROY     = 32,
   GUI_TOPMOST       = 64,
   GUI_BORDER        = 128,
   GUI_BACKGROUND    = 256,
   GUI_FOCUSED       = 512,
   GUI_CAPTION       = 1024,
   GUI_LIST_HEADER   = 2048,
   GUI_HORZ_SCROLL   = 4096,
   GUI_POPUP         = 8192,
   GUI_HOOVER        = 16384
};

enum GuiState {
   GuiCtrl                    = 1,
   GuiShift                   = 2,
   GuiAlt                     = 4,
   GuiLButton                 = 8,
   GuiRButton                 = 16,
   GuiMButton                 = 32
};

enum GuiEvent {
   GuiWndCreatedEvent         = 0,
   GuiWndContextMenuEvent     = 11,
   GuiWndContextCommandEvent  = 13,
   GuiButtonClickEvent        = 1,
   GuiScrollUpEvent           = 2,
   GuiScrollDownEvent         = 3,
   GuiScrollPosChangedEvent   = 4,
   GuiLostFocusEvent          = 5,
   GuiListSelChangeEvent      = 6,
   GuiListEditLostFocusEvent  = 7,
   GuiListKeyPressEvent       = 12,
   GuiHeaderResizeEvent       = 8,
   GuiContextMenuEvent        = 9,
   GuiDialogLoadedEvent       = 10,
   GuiTreeSelChangeEvent      = 14,

   GuiTileEditorMouseDownEvent  = 15,
   GuiTileEditorMouseClickEvent = 16,
   GuiTileEditorMouseMoveEvent  = 17,
   GuiTileEditorKeyPressEvent   = 18,

   UndefinedEvent             = 0xFFFF
};

enum GuiControlIds
{
   GuiInvalidId               = -1,
   GuiLabelId                 = 1,
   GuiButtonId                = 2,
   GuiEditBoxId               = 3,
   GuiImageButtonId           = 4,
   GuiComboBoxId              = 5,
   GuiListBoxId               = 6,
   GuiCheckBoxId              = 7,
   GuiImageId                 = 8,
   GuiMovieId                 = 9,
   GuiTileEditorId            = 10,
   GuiTileImageListBoxId      = 11,
   GuiTreeBoxId               = 12,
   GuiFileListBoxId           = 13
};

// Event declarations for the event maps

#define EVENT_MAP_BEGIN(className)                          \
   void className::sendMessage(GuiId id, GuiEvent event, int param1) {  \
   if (id == -1 || event == -1) { return; }
#define EVENT_MAP_END() }

#define ON_LOST_FOCUS(i,fn)                                             \
   else if (i == id && event==GuiLostFocusEvent) { fn(); }

#define ON_BTN_CLICK(i,fn)                                              \
   else if (i == id && event==GuiButtonClickEvent) { fn(); }
#define ON_SB_UP(i,fn)                                                  \
   else if (i == id && event==GuiScrollUpEvent) { fn(param1); }
#define ON_SB_DOWN(i,fn)                                                \
   else if (i == id && event==GuiScrollDownEvent) { fn(param1); }
#define ON_SB_POSCHANGED(i,fn)                                          \
   else if (i == id && event==GuiScrollPosChangedEvent) { fn(param1); }

#define ON_LIST_SELCHANGE(i,fn)                                         \
   else if (i == id && event==GuiListSelChangeEvent) { fn(param1); }
#define ON_LIST_EDITLOSTFOCUS(i,fn)                                     \
   else if (i == id && event == GuiListEditLostFocusEvent) { fn(); }

#define ON_HEADER_RESIZE(i,fn)  else if (i == id && event == GuiHeaderResizeEvent) { fn(param1); }

#define ON_COMMAND(i,fn)   else if ( i == id && event == GuiContextMenuEvent) { fn(param1); }

// Designer registration

#define DESIGNER_REGISTRATION(class)                                                                  \
   static GuiControl* createControl(int id, const GuiPoint& pos, GuiWnd* pparent);                    \
   static int _##class##_id;                                                                          \
   virtual int getTypeId() const;

#define REGISTER_DESIGNER(class,id,label,width,height,style)                                          \
   int class::_##class##_id = GuiDesigner::registerControl(id, #class, label, class::createControl);  \
   GuiControl* class::createControl(int id, const GuiPoint& pos, GuiWnd* pparent)                     \
   {                                                                                                  \
      class* pcontrol = new class();                                                                  \
      pcontrol->create(id, GuiRect(pos.x, pos.x+width, pos.y, pos.y+height), label, style, pparent);  \
      return pcontrol;                                                                                \
   }                                                                                                  \
   int class::getTypeId() const { return _##class##_id; }

// Property registration

#define DECLARE_PROPERTIES                                                                            \
   protected:                                                                                         \
      virtual void   initializeProperties();

#define IMPLEMENT_PROPERTIES(class,base)                                                              \
   void class::initializeProperties() {                                                               \
      base::initializeProperties();

#define INIT_PROPERTY(name)                                                                           \
   _p##name(NULL)

#define CONSTRUCT_STR_PROPERTY(name, desc, default)                                                   \
   _p##name = new GuiProperty<std::string>(desc, default, GuiPropertyBase::EString);                  \
   MProperties.Add(_p##name);

#define CONSTRUCT_STR_PROPERTY_WITH_CALLBACK(name, desc, default, callback)                           \
   CONSTRUCT_STR_PROPERTY(name, desc, default)                                                        \
   _p##name->registerCallback((GuiPropertyBase::CallbackFnc)callback);

#define CONSTRUCT_BOOL_PROPERTY(name, desc, default)                                                  \
   _p##name = new GuiProperty<bool>(desc, default, GuiPropertyBase::EBool);                           \
   MProperties.Add(_p##name);

#define CONSTRUCT_BOOL_PROPERTY_WITH_CALLBACK(name, desc, default, callback)                          \
   CONSTRUCT_BOOL_PROPERTY(name, desc, default)                                                       \
   _p##name->registerCallback((GuiPropertyBase::CallbackFnc)callback);

#define STR_PROPERTY(name)                                                                            \
   public:                                                                                            \
   GuiPropertyBase& get##name##Property() { return *_p##name; }                                       \
      const std::string& get##name() const { return _p##name->Value(); }                            \
      std::string&       get##name() { return _p##name->Value(); }                                  \
      void               set##name(const std::string& value) { _p##name->Value(value); }            \
   private:                                                                                           \
      GuiProperty<std::string>*  _p##name;

#define BOOL_PROPERTY(name)                                                                           \
   public:                                                                                            \
      bool get##name() const { return _p##name->Value(); }                                           \
      void set##name(bool value) { _p##name->Value(value); }                                        \
   private:                                                                                           \
      GuiProperty<bool>* _p##name;

#endif

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
#include <tinyxml.h>

#include "../defines.h"
#include "../game.h"

#include "gui/edit/guiedit.h"

#include "guidesigner.h"
#include "guidesigndecorator.h"
#include "guidesignselection.h"
#include "guieventhandlerdefinitions.h"
#include "guidesignwnd.h"
#include "guicontrol.h"
#include "guimanager.h"
#include "guimenu.h"
#include "guipopupdecorator.h"

EVENT_MAP_BEGIN(GuiDesignWnd)
   ON_LOST_FOCUS(21, onMenuLostFocus)
EVENT_MAP_END()

GuiDesignWnd::GuiDesignWnd():
   GuiDialog(),
   _pselectionctrl(NULL),
   _popupMenu(NULL),
   mMouseListener(*this),
   mMotionListener(*this),
   _functions(),
   _filename(),
   _selected(false)
{
}

GuiDesignWnd::~GuiDesignWnd()
{
   if ( _popupMenu != NULL )
   {
      delete _popupMenu;
      _popupMenu = NULL;
   }
}

void GuiDesignWnd::onCreate(const GuiRect& rect, const char* caption, GuiStyle style, GuiWnd* parent)
{
   setCenter(false);
   GuiDialog::onCreate(rect, caption, style, parent);

   GuiRect selrect(getWindowRect());
   windowToClient(selrect);

   _pselectionctrl = new GuiDesignSelection();
   _pselectionctrl->create(0, selrect, "", 0, this);
   _pselectionctrl->setVisible(false);

   addMouseListener(mMouseListener);
   addMouseMotionListener(mMotionListener);
}

//////////////////////////////////////////////////////////////////////////
// - Input interface
//////////////////////////////////////////////////////////////////////////

bool GuiDesignWnd::onContextMenu(const GuiPoint& point)
{
   GuiDesigner* pdesigner = dynamic_cast<GuiDesigner*>(getParent());
   ASSERT_PTR(pdesigner)
   
   if ( _popupMenu == NULL )
   {
            GuiMenu& menu = pdesigner->getWindowPopup();
      const GuiRect& rect = menu.getWindowRect();

      _popupMenu = new GuiPopupDecorator(menu);
      _popupMenu->create(100, GuiRect(point.x, point.x + rect.getWidth(), point.y, point.y + rect.getHeight()), "", GUI_VISIBLE, NULL);
      _popupMenu->setOwner(*this);
   }
   else
   {
      _popupMenu->setWindowPos(point.x, point.y);
   }

   //_popupMenu->setParent(this);
   _popupMenu->popup();

   return true;
}

//////////////////////////////////////////////////////////////////////////
// - Overloaded operations
//////////////////////////////////////////////////////////////////////////

void GuiDesignWnd::onResize(int width, int height)
{
   ASSERT_PTR(_pselectionctrl)

   _pselectionctrl->resizeWindow(width, height);
}

void GuiDesignWnd::onSetFocus(GuiWnd* oldCtrl)
{
   GuiDialog::onSetFocus(oldCtrl);

   unselectAll();

   _pselectionctrl->setVisible(true);
}

void GuiDesignWnd::onKillFocus(GuiWnd* newCtrl)
{
   GuiDialog::onKillFocus(newCtrl);

   _pselectionctrl->setVisible(false);
}

//////////////////////////////////////////////////////////////////////////
// - Operations
//////////////////////////////////////////////////////////////////////////

void GuiDesignWnd::onMenuLostFocus()
{
   _popupMenu->hide();
}

void GuiDesignWnd::onCommand(int id)
{
   GuiPoint p(_popupMenu->getWindowRect().left(), _popupMenu->getWindowRect().top());
   windowToClient(p);

   _popupMenu->hide();
   _popupMenu->setParent(NULL);

   if ( id == 50 )
   {
      save();
   }
   else if ( id == 51 )
   {
      close();
   }
   else
   {
      GuiDesignDecorator* pdec = new GuiDesignDecorator();
      pdec->create(3, GuiRect(), "decorator", 8, this);

      GuiControl* pcontrol = GuiDesigner::createControl(id, 0, p, pdec);
      ASSERT_PTR(pcontrol)
      pdec->control(pcontrol);
   }
}

int GuiDesignWnd::selectionSize() const
{
   int count = 0;
   GuiList::Iterator it = childs.begin();
   for ( ; it.valid(); ++it )
   {
      GuiDesignDecorator* pcontrol = dynamic_cast<GuiDesignDecorator*>(*it);
      if ( pcontrol && pcontrol->isSelected() )
         ++count;
   }
   return count;
}

void GuiDesignWnd::unselectAll()
{
   GuiList::Iterator it = childs.begin();
   for ( ; it.valid(); ++it )
   {
      GuiDesignDecorator* pcontrol = dynamic_cast<GuiDesignDecorator*>(*it);
      if ( pcontrol != NULL )
         pcontrol->setSelected(false);
   }

   _pselectionctrl->setVisible(false);
}

void GuiDesignWnd::deleteSelected()
{
   GuiList::Iterator it = childs.begin();
   for ( ; it.valid(); ++it )
   {
      GuiDesignDecorator* pcontrol = dynamic_cast<GuiDesignDecorator*>(*it);
      if ( pcontrol && pcontrol->isSelected() )
      {
         if ( pcontrol->hasFocus() )
            setFocus();

         childs.removeAt(it);
         delete pcontrol;
      }
   }
}

void GuiDesignWnd::moveSelected(const GuiPoint& rel)
{
   GuiList::Iterator it = childs.begin();
   for ( ; it.valid(); ++it )
   {
      GuiDesignDecorator* pcontrol = dynamic_cast<GuiDesignDecorator*>(*it);
      if ( pcontrol && pcontrol->isSelected() )
         pcontrol->moveWindow(rel.x, rel.y);
   }
}

void GuiDesignWnd::resizeSelected(const GuiPoint& rel, int borders)
{
   if ( selectionSize() == 1 )
   {
      GuiList::Iterator it = childs.begin();
      for ( ; it.valid(); ++it )
      {
         GuiDesignDecorator* pcontrol = dynamic_cast<GuiDesignDecorator*>(*it);
         if ( pcontrol && pcontrol->isSelected() )
            pcontrol->resize(rel, borders);
      }
   }
}

void GuiDesignWnd::close()
{
   GuiDesigner* pdesigner = dynamic_cast<GuiDesigner*>(getParent());
   ASSERT_PTR(pdesigner)

   pdesigner->close(this);
}

void GuiDesignWnd::align(GuiWnd& main, GuiDesignAlign how)
{
   const GuiRect& mainrect = main.getWindowRect();

   GuiList::Iterator it = childs.begin();
   for ( ; it.valid(); ++it )
   {
      GuiDesignDecorator* pcontrol = dynamic_cast<GuiDesignDecorator*>(*it);
      if ( pcontrol && pcontrol->isSelected() && pcontrol != &main )
      {
         const GuiRect& rect = pcontrol->getWindowRect();
         switch ( how )
         {
         case alignLeft  : pcontrol->setWindowPos(mainrect.left(), rect.top());                        break;
         case alignRight : pcontrol->setWindowPos(mainrect.right() - rect.getWidth(), rect.top());     break;
         case alignTop   : pcontrol->setWindowPos(rect.left(), mainrect.top());                        break;
         case alignBottom: pcontrol->setWindowPos(rect.left(), mainrect.bottom() - rect.getHeight());  break;
         default         : ASSERT(false);
         }
      }
   }
}

void GuiDesignWnd::sameSize(GuiWnd& main, GuiDesignSize how)
{
   const GuiRect& mainrect = main.getWindowRect();

   GuiList::Iterator it = childs.begin();
   for ( ; it.valid(); ++it )
   {
      GuiDesignDecorator* pcontrol = dynamic_cast<GuiDesignDecorator*>(*it);
      if ( pcontrol && pcontrol->isSelected() && pcontrol != &main )
      {
         const GuiRect& rect = pcontrol->getWindowRect();
         switch ( how )
         {
         case sizeWidth : pcontrol->resizeWindow(mainrect.getWidth(), rect.getHeight()); break;
         case sizeHeight: pcontrol->resizeWindow(rect.getWidth(), mainrect.getHeight()); break;
         default        : ASSERT(false)
         }
      }
   }
}

//////////////////////////////////////////////////////////////////////////
// - Save/load interface
//////////////////////////////////////////////////////////////////////////

void GuiDesignWnd::save()
{
   if ( _filename.empty() )
   {
      GuiDialog* pdialog = GuiManager::getInstance().loadDialogFromXML("openfile");
      pdialog->center();
      if ( pdialog->doModal() == 0 )
      {
         GuiEditBox* pedit  = dynamic_cast<GuiEditBox*>(pdialog->findChildByName("edtFile"));
         _filename = "../scripts/" + pedit->getCaption() + ".xml";
      }
      else
      {
         return;
      }
   }

   doSave();
}

void GuiDesignWnd::doSave()
{
   TiXmlDocument doc;
	TiXmlDeclaration* decl = new TiXmlDeclaration("1.0", "", "");
   TiXmlElement* pdialog  = new TiXmlElement("dialog");
   TiXmlElement* pself    = new TiXmlElement("self");

   pself->LinkEndChild(getProperties().toXML());
   pself->LinkEndChild(getEventHandlerDefinitions().toXML());
   const GuiRect& rect = getWindowRect();
   pself->SetAttribute("x", rect.left());
   pself->SetAttribute("y", rect.top());
   pself->SetAttribute("width", rect.getWidth());
   pself->SetAttribute("height", rect.getHeight());

   TiXmlElement* children = new TiXmlElement("children");
   GuiList::Iterator it = childs.begin();
   for ( ; it.valid(); ++it )
   {
      TiXmlElement* pchild = new TiXmlElement("child");

      GuiDesignDecorator* pcontrol = dynamic_cast<GuiDesignDecorator*>(*it);
      if ( pcontrol != NULL )
      {
         GuiControl& actualcontrol = pcontrol->control();

         int type_id = actualcontrol.getTypeId();
         pchild->SetAttribute("type", type_id);

         const GuiRect& rect = pcontrol->getWindowRect();
         pchild->SetAttribute("x", rect.left());
         pchild->SetAttribute("y", rect.top());
         pchild->SetAttribute("width", rect.getWidth());
         pchild->SetAttribute("height", rect.getHeight());

         pchild->LinkEndChild(actualcontrol.getProperties().toXML());
         pchild->LinkEndChild(actualcontrol.getEventHandlerDefinitions().toXML());

         children->LinkEndChild(pchild);
      }
   }

   pdialog->LinkEndChild(pself);
   pdialog->LinkEndChild(children);

   if ( !_functions.empty() )
   {
      TiXmlElement* pfunctions = new TiXmlElement("functions");
      for ( int functionid = 0; functionid < _functions.size(); ++functionid )
      {
         TiXmlText* pfunctionbody = new TiXmlText(_functions[functionid]);
         TiXmlElement* pfunction = new TiXmlElement("eventhandler");
         pfunction->LinkEndChild(pfunctionbody);
         pfunctions->LinkEndChild(pfunction);
      }

      pdialog->LinkEndChild(pfunctions);
   }

   doc.LinkEndChild(decl);
   doc.LinkEndChild(pdialog);
	doc.SaveFile(_filename);
}

void GuiDesignWnd::load(const std::string& file)
{
   TiXmlDocument doc;

   if ( doc.LoadFile(file) )
   {
      _filename = file;

      TiXmlElement* pdialog = (TiXmlElement*)doc.FirstChild("dialog");

      int type, x, y, width, height;
      TiXmlElement* pself = (TiXmlElement*)pdialog->FirstChild("self");
      getProperties().fromXML(pself);
      getEventHandlerDefinitions().fromXML(pself);

      pself->QueryIntAttribute("x", &x);
      pself->QueryIntAttribute("y", &y);
      pself->QueryIntAttribute("width", &width);
      pself->QueryIntAttribute("height", &height);

      setWindowPos(x, y);
      resizeWindow(width, height);

      TiXmlElement* pchildren = (TiXmlElement*)pdialog->FirstChild("children");
      TiXmlElement* pchild = (TiXmlElement*)pchildren->FirstChild("child");
      while ( pchild )
      {
         pchild->QueryIntAttribute("type", &type);
         pchild->QueryIntAttribute("x", &x);
         pchild->QueryIntAttribute("y", &y);
         pchild->QueryIntAttribute("width", &width);
         pchild->QueryIntAttribute("height", &height);

         GuiPoint p(x,y);
         GuiDesignDecorator* pdec = new GuiDesignDecorator();
         pdec->create(3, GuiRect(), "decorator", 8, this);
         pdec->control(GuiDesigner::createControl(type, 0, p, pdec));
         pdec->resizeWindow(width, height);

         pdec->control().getProperties().fromXML(pchild);
         pdec->control().getEventHandlerDefinitions().fromXML(pchild);

         pchild = (TiXmlElement*)pchildren->IterateChildren("child", pchild);
      }

      TiXmlElement* pfunctions = (TiXmlElement*)pdialog->FirstChild("functions");
      if ( pfunctions != NULL )
      {
         TiXmlElement* pfunction = (TiXmlElement*)pfunctions->FirstChild("function");
         while ( pfunction != NULL )
         {
            TiXmlText* pcode = (TiXmlText*)pfunction->FirstChild();
            const std::string& value = pcode->Value();
            _functions.push_back(value);

            pfunction = (TiXmlElement*)pfunctions->IterateChildren("function", pfunction);
         }
      }
   }
}

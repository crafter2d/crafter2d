/***************************************************************************
 * Copyright (C) 2006 Jeroen Broekhuizen                                   *
 * jeroen@jeroen.homedns.org                                               *
 *                                                                         *
 * This library is free software; you can redistribute it and/or           *
 * modify it under the terms of the GNU Lesser General Public              *
 * License as published by the Free Software Foundation; either            * 
 * version 2.1 of the License, or (at your option) any later version.      *
 *                                                                         *
 * This library is distributed in the hope that it will be useful,         *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of          *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU       *
 * Lesser General Public License for more details.                         *
 *                                                                         *
 * You should have received a copy of the GNU Lesser General Public        *
 * License along with this library; if not, write to the                   *
 * Free Software Foundation, Inc.,                                         *
 * 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA            *
 ***************************************************************************/

#include "guieventhandlerdefinition.h"
#include "guieventhandlerdefinitions.h"
#include "guilistbox.h"
#include "guiedit.h"

#include "guieventhandlerdlg.h"
#ifndef JENGINE_INLINE
#  include "guieventhandlerdlg.inl"
#endif

EVENT_MAP_BEGIN(GuiEventHandlerDlg)
   ON_LIST_SELCHANGE(1, onSelChanged)
EVENT_MAP_END()

GuiEventHandlerDlg::GuiEventHandlerDlg():
   _pcurrentWnd(NULL),
   _plist(NULL),
   _pedit(NULL),
   _currentEdit(-1)
{
}

void GuiEventHandlerDlg::onCreate (const GuiRect& rect, const char* caption, GuiStyle style, GuiWnd* parent)
{
   GuiDialog::onCreate(rect, caption, style, parent);

   _plist = new GuiListBox();
   _plist->create(1, GuiRect(5, 195, 25, 125), "", 392, this);

   _pedit = new GuiEditBox();
   _pedit->create(2, GuiRect(5, 195, 130, 230), "", 392, this);
   _pedit->setMultiLine(true);
}

//////////////////////////////////////////////////////////////////////////
// - Callbacks
//////////////////////////////////////////////////////////////////////////

void GuiEventHandlerDlg::onSelChanged(int sel)
{
   saveCode();

   GuiEventHandlerDefinitions& definitions = _pcurrentWnd->getEventHandlerDefinitions();
   GuiEventHandlerDefinitions::Definitions& defs = definitions.getDefinitions();

   int def = _plist->getItemData(_currentEdit);
   GuiEventHandlerDefinition* pdefinition = defs[def];

   _pedit->setCaption(pdefinition->getCode().c_str());
   _currentEdit = sel;
}

//////////////////////////////////////////////////////////////////////////
// - Operations
//////////////////////////////////////////////////////////////////////////

void GuiEventHandlerDlg::setCurrentWnd(GuiWnd* pwnd)
{
   saveCode();

   _pcurrentWnd = pwnd;
   _currentEdit = -1;
   _pedit->setCaption("");

   GuiEventHandlerDefinitions& definitions = pwnd->getEventHandlerDefinitions();
   GuiEventHandlerDefinitions::Definitions& defs = definitions.getDefinitions();

   _plist->removeAll();
   for ( int def = 0; def < defs.size(); ++def )
   {
      GuiEventHandlerDefinition* pdefinition = defs[def];

      int index = _plist->addString(pdefinition->getName().c_str());
      //_plist->setItemData(index, (Uint32)pdefinition);
      _plist->setItemData(index, def);
   }
}

void GuiEventHandlerDlg::saveCode()
{
   if ( _currentEdit >= 0 )
   {
      GuiEventHandlerDefinitions& definitions = _pcurrentWnd->getEventHandlerDefinitions();
      GuiEventHandlerDefinitions::Definitions& defs = definitions.getDefinitions();

      int def = _plist->getItemData(_currentEdit);
      GuiEventHandlerDefinition* pdefinition = defs[def];
      pdefinition->setCode(_pedit->getCaption());
   }
}

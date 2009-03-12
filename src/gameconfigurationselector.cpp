/***************************************************************************
 *   Copyright (C) 2006 by Jeroen Broekhuizen                              *
 *   jeroen@jengine.homedns.org                                            *
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
#include "gameconfigurationselector.h"

#include "gui/guidialog.h"
#include "gui/guilistbox.h"
#include "gui/guimanager.h"

#include "autoptr.h"
#include "game.h"
#include "gameconfiguration.h"

GameConfigurationSelector::GameConfigurationSelector(Game& game):
   mGame(game),
   mManager(),
   mpSelectedConfiguration(NULL)
{
}

GameConfigurationSelector::~GameConfigurationSelector()
{
}

//////////////////////////////////////////////////////////////////////////
// - get/set
//////////////////////////////////////////////////////////////////////////

GameConfiguration& GameConfigurationSelector::getSelectedConfiguration()
{
   ASSERT_PTR(mpSelectedConfiguration);
   return *mpSelectedConfiguration;
}

//////////////////////////////////////////////////////////////////////////
// - operations
//////////////////////////////////////////////////////////////////////////

bool GameConfigurationSelector::select()
{
   mManager.initialize();
   GameConfigurationManager::GameConfigurations& configurations = mManager.getConfigurations();

   if ( configurations.size() == 1 )
   {
      mpSelectedConfiguration = configurations[0];
   }
   else if ( configurations.size() > 1 )
   {
      AutoPtr<GuiDialog> pdialog = GuiManager::getInstance().loadSystemDialog("gen_selectgame");
      ASSERT(pdialog.hasPointer());

      GuiListBox* plist = dynamic_cast<GuiListBox*>(pdialog->findChildByName("lstGames"));
      ASSERT_PTR(plist);

      for ( unsigned int index = 0; index < configurations.size(); index++ )
      {
         GameConfiguration* pconfiguration = configurations[index];
         plist->addString(pconfiguration->getName().c_str());
      }

      if ( pdialog->doModal() == 0 )
      {
         int index = plist->getCurSel();
         GameConfiguration* pconfiguration = configurations[index];
         mpSelectedConfiguration = pconfiguration;
      }
   }

   return mpSelectedConfiguration != NULL;
}

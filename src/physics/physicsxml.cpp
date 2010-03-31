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
#include "physicsxml.h"

#include <tinyxml.h>

#include "../autoptr.h"
#include "../defines.h"

#include "physicsbody.h"
#include "collisionbox.h"
#include "collisioncircle.h"
#include "collisionplane.h"

const std::string sBODYELEMENT       = "body";
const std::string sSHAPEELEMENT      = "shape";
const std::string sTYPE              = "type";

Body* PhysicsXML::parseXML(TiXmlElement& object)
{
   TiXmlElement* pphysics = dynamic_cast<TiXmlElement*>(object.FirstChild(sBODYELEMENT));
   if ( pphysics == NULL )
   {
      return NULL;
   }

   AutoPtr<Body> body = NULL;

   const std::string* psimulationtype = pphysics->Attribute(sTYPE);
   if ( psimulationtype != NULL )
   {
      if ( psimulationtype->compare("physics") == 0 )
      {
         PhysicsBody* pbody = new PhysicsBody();

         float mass, inertia;
         pphysics->QueryFloatAttribute("mass", &mass);
         pphysics->QueryFloatAttribute("inertia", &inertia);

         pbody->setMass(mass);
         pbody->setInertia(inertia);

         body = pbody;
      }
   }

   TiXmlElement* pshapeelement = dynamic_cast<TiXmlElement*>(pphysics->FirstChild(sSHAPEELEMENT));
   if ( pshapeelement != NULL )
   {
      const std::string* pshapetype = pshapeelement->Attribute(sTYPE);
      if ( pshapetype != NULL )
      {
         CollisionShape* pshape = NULL;
         if ( pshapetype->compare("box") == 0 )
         {
            Vector halfsize;
            pshapeelement->QueryFloatAttribute("halfx", &halfsize.x);
            pshapeelement->QueryFloatAttribute("halfy", &halfsize.y);

            pshape = CollisionBox::construct(halfsize);
         }
         else if ( pshapetype->compare("circle") == 0 )
         {
            float radius = 0.0f;
            pshapeelement->QueryFloatAttribute("radius", &radius);
            pshape = CollisionCircle::construct(radius);
         }

         body->addShape(pshape);
      }
   }

   return body.release();
}

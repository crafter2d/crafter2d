#pragma once

/***************************************************************************
*   Copyright (C) 2010 by Jeroen Broekhuizen                              *
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
#ifndef MOD_D3D12_H_
#define MOD_D3D12_H_

#if defined(WIN32)
  #define MOD_DECL cdecl
  #ifdef MOD_EXPORTS
    #define MOD_API __declspec(dllexport)
    #define MOD_TEMPLATE
  #else
    #define MOD_API __declspec(dllimport)
    #define MOD_TEMPLATE extern
  #endif
#else
  #define MOD_DECL
  #define MOD_API
  #define MOD_TEMPLATE
#endif

#endif // MOD_D3D12_H_

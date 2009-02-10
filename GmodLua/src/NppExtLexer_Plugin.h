// NppExternalLexer_Plugin.h
// This file is part of GmodLua.
// Written for Notepad++

// Copyright (C)2008-2009 Kyle Fleming ( garthex@gmail.com )

// The following code is presented in full, written by Thell Fowler
// for the Notepad++ External Lexers Plugin.

///////////////////////////////////////////////////////////////////////////////////////////////

#ifndef NPPEXTLEXER_PLUGIN_H
#define NPPEXTLEXER_PLUGIN_H

// NppExtLexer_Plugin.h

// This file is part of the Notepad++ External Lexers Plugin.
// Copyright 2008 - 2009 Thell Fowler (thell@almostautomated.com)
//
// This program is free software; you can redistribute it and/or modify it under the terms of
// the GNU General Public License as published by the Free Software Foundation; either version
// 2 of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
// without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
// See the GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License along with this program;
// if not, write to the Free Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.


///////////////////////////////////////////////////////////////////////////////////////////////
//                    Non-Language Specific Parameters
//              for use with Notepad++ External Lexers Plugin
///////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////
//  Include Directives

#include "NppExtLexerInterface.h"


namespace NppExtLexer_Plugin {

///////////////////////////////////////////////////////////////////////////////////////////////
//  Global (within this namespace) Declarations


//*********************************************************************************************
//  Public Functions


//  <--- Menu function items. --->
//  These are the plugin's non-language specific menu items.
void Help_func();
void About_func();

}  // End: namespace NppExtLexer_Plugin


#endif	//  Include Guard:  NPPEXTLEXER_PLUGIN_H

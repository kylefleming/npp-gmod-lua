// NppExtLexerInterface.h
// This file is part of GmodLua.
// Written for Notepad++

// Copyright (C)2008-2010 Kyle Fleming ( garthex@gmail.com )

// The following code is presented in full, written by Thell Fowler
// for the Notepad++ External Lexers Plugin.

///////////////////////////////////////////////////////////////////////////////////////////////

#ifndef NPP_PLUGININTERFACE_EXTLEXER_EXTENSION_H
#define NPP_PLUGININTERFACE_EXTLEXER_EXTENSION_H

// NppExtLexerInterface.h

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
//                       Public Plugin Interface Extension
//				   for use with Notepad++ External Lexer Plugins
///////////////////////////////////////////////////////////////////////////////////////////////

	/***
	 *
	 *  This file extends the NppPluginInterface for the purposes of using Scintilla's
	 *  external lexer capabilities in a Notepad++ plugin.
	 *
	 *  The commenting in this code may seem excessive, yet it is there for people who would
	 *  like to better understand how the plugin system works and how it can be used.
	 *
	 ***/

///////////////////////////////////////////////////////////////////////////////////////////////
//  Include Directives

#include "NppPluginInterface.h"
								/*
								 *  Defines the interface for interacting with Notepad++
								 *  Includes:
								 *    # <vector>
								 *    # <algorithm>
								 *    # <iterator>
								 *    # "PluginInterface.h"
								 *
								 *  These files define most items needed for plugin messaging
								 *  and setup/registration with Notepad++'s plugin manager.
								 *
								 */

#include "Platform.h"
								/*
								 *  Scintilla platform definitions
								 *  Required for Window Accessor
								 *  No additional includes come from this file.
								 *
								 */

#include "SciLexer.h"			//  Included for SCLEX_AUTOMATIC; no additional includes.


///////////////////////////////////////////////////////////////////////////////////////////////
//  Main public plugin extension interface definitions.

// <--- External LexOrFold Function Definition --->
typedef void (*NppExtLexerFunction)(bool LexOrFold, unsigned int startPos, int lengthDoc, int initStyle,
                  char *words[], WindowID window, char *props);

//  <---  Namespace --->
namespace Npp_ExtLexer_Interface {

//  <--- Structures --->
struct Lexer
{

	std::string _name;			// Use of char instead of TCHAR since Scintilla expects char.
	tstring _description;
	NppExtLexerFunction _pLexOrFold;
	int SCI_LEXERID;

};


//*********************************************************************************************
//  Public Functions

//  <--- Initialization --->
void init(std::string Name, tstring statusText, NppExtLexerFunction pLexOrFold,
			   PFUNCPLUGINCMD pMenuDlg);		//  Setup a lexer definition.

void setLexerFuncItem(tstring Name, PFUNCPLUGINCMD pFunction, int cmdID = NULL,
				bool init2Check = false, ShortcutKey* pShKey = NULL);	//  Store additional lexer FuncItem commands.

//  <--- Data Retrieval --->
int getSCILexerIDByIndex( int index );			//  Returns the Scintilla lexer ID for this vector index.
int getSCILexerIDByName( std::string name );	//  Returns the Scintilla lexer ID for vector index matching name.
std::vector<FuncItem> getLexerFuncVector();		//  Return a copy of this extensions Function vector.
std::vector<Lexer> getLexerDetailVector();		//  Return a copy of this extensions Lexer vector.

//  'Virtualized' base plugin FuncItem functions. 
int getPluginFuncCount();						//  'Virtualize' the base plugin's getPluginFuncCount() function.
FuncItem * getPluginFuncArray();				//  'Virtualize' the base plugin's getPluginFuncArray() function.


}  // End namespace:  Npp_ExtLexer_Interface.

#endif  // End include guard: NPP_PLUGININTERFACE_EXTLEXER_EXTENSION_H
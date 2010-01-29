// NppPluginInterface.h
// This file is part of GmodLua.
// Written for Notepad++

// Copyright (C)2008-2010 Kyle Fleming ( garthex@gmail.com )

// The following code is presented in full, written by Thell Fowler
// for the Notepad++ External Lexers Plugin.

///////////////////////////////////////////////////////////////////////////////////////////////

#ifndef NPP_PLUGININTERFACE_H
#define NPP_PLUGININTERFACE_H

// NppPluginInterface.h

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
//                   Public Plugin Interface Definition
//				       for use with Notepad++ Plugins
///////////////////////////////////////////////////////////////////////////////////////////////

	/***
	 *
	 *  This file provides an enhanced interface to the standard Notepad++ plugin interface
	 *  defined in PluginInterface.h using a namespace with public functions defined in this
	 *  header and private ones in the corresponding source file under an unnamed namespace.
	 *
	 *  The commenting in this code may seem excessive, yet it is there for people who would
	 *  like to better understand how the plugin system works and how it can be used.
	 *
	 ***/

///////////////////////////////////////////////////////////////////////////////////////////////
//  Include Directives

	/***
	 *
	 *  The includes provided here are for the base plugin interface.  To extend the plugin
	 *  interface include a header in your project that extends the namespace.
	 *
	 *  An example of this is the NppExtLexer_Interface.h which 'virtualizes' some features in
	 *  the namespace for use with Scintilla's external lexer api.
	 *
	 ***/

//*********************************************************************************************
//  Library includes.

// <--- STL --->
#include <vector>               //  Dynamic array container for Function Items.
#include <algorithm>            //  Utility functions for working with the vectors.
#include <iterator>             //  Iterator for merging the vectors, and crtdgb.h for _ASSERT.
#include <TCHAR.h>				//  Provides MS specific routines for working with Unicode/ANSI.
#include <windows.h>			//	Master include for Windows.

// <--- Notepad++ --->
#include "PluginInterface.h"
								/*
								 *  Defines the interface for interacting with Notepad++
								 *  Includes:
								 *    # "Scintilla.h" Main include for Scintilla definitions.
								 *    # "Notepad_plus_msgs.h" Main include file for Notepad++
								 *       messaging
								 *
								 *  These files define most items needed for plugin messaging.
								 *
								 */


///////////////////////////////////////////////////////////////////////////////////////////////
//  Main public plugin interface definitions.

//  Since Npp uses either ansi or unicode, we might as well have a basic TCHAR string type.
typedef std::basic_string< TCHAR > tstring;

//  Might as well also have a MACRO to define which memcpy we'll use when needed.
#ifdef UNICODE
	#define TMEMCPY wmemcpy
#else
	#define TMEMCPY memcpy
#endif


//  <---  Namespace --->
namespace Npp_Plugin_Interface {


//*********************************************************************************************
//  Public Functions


//  <--- Initialization --->
void init(tstring name, HANDLE hModule);			//  Initialize Plugin.

void setPluginFuncItem(tstring Name, PFUNCPLUGINCMD pFunction, int cmdID = NULL,
	   bool init2Check = false, ShortcutKey* pShKey = NULL); //  Store a FuncItem command.

void setNppReady();									//  Sets flag when Notepad++ is done with start up.
void hCurrViewNeedsUpdate();						//  Sets flag to update hCurrView.


//  <--- Data Retrieval --->
HANDLE hModule();									//  Plugin module's handle.
HWND hNpp();										//  Returns the main Notepad++ handle.
HWND hMainView();									//  Returns handle used in messaging the main view.
HWND hSecondView();									//  Returns handle used in messaging the secondary view.
HWND hCurrView();									//  Returns handle for the currently focused view.
int intCurrView();									//  Returns 0 for main view and 1 for second view.
bool isNppReady();									//  Returns Notepad++ initialization state.

int getPluginFuncCount();							//  Returns the size of the Function vector.
std::vector<FuncItem> getPluginFuncVector();		//  Return a copy of the plugins function vector.
FuncItem * getPluginFuncArray();					//  Returns this plugin's Function vector as an array.

//*********************************************************************************************
//  Exported Functions

	/*
	 *  These are the required exported functions that are handled directly by this interface.
	 *
	 */

extern "C" __declspec(dllexport) const TCHAR * getName();

}  // End Namespace: Npp_Plugin_Interface

#endif // End include guard:  NPP_PLUGININTERFACE_H
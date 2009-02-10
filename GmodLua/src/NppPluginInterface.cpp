// NppPluginInterface.cpp
// This file is part of GmodLua.
// Written for Notepad++

// Copyright (C)2008-2009 Kyle Fleming ( garthex@gmail.com )

// The following code is presented in full, written by Thell Fowler
// for the Notepad++ External Lexers Plugin.

///////////////////////////////////////////////////////////////////////////////////////////////

// NppPluginInterface.cpp

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
//				Public and Private Plugin Interface Functions
//				       for use with Notepad++ Plugins
///////////////////////////////////////////////////////////////////////////////////////////////

	/***
	 *
	 *  This file implements an alternate interface to the standard Notepad++ plugin interface,
	 *  using a namespace with public functions defined in the header and private ones in an
	 *  unnamed namespace in this file.
	 *
	 *  The commenting in this code may seem excessive, yet it is there for people who would
	 *  like to better understand how the plugin system works and how it can be used.
	 *
	 ***/

///////////////////////////////////////////////////////////////////////////////////////////////
//  Include Directives

	/***
	 *
	 *  The includes provided here are for the base plugin interface.
	 *  To extend the plugin interface include a header that extends the namespace.
	 *
	 *  An example of this is the NppPluginInterface_ExternalLexer.h which adds features to
	 *  the namespace for use with Scintilla's external lexer api.
	 *
	 ***/

# include "NppPluginInterface.h"  //  Provides definitions for the public plugin interface.

//---------------------------------------------------------------------------------------------
//  Additional library includes.

#include <algorithm>            //  Utility functions for working with the vectors.
#include <iterator>             //  Iterator for merging the vectors, and crtdgb.h for _ASSERT.


///////////////////////////////////////////////////////////////////////////////////////////////
//  Main interface implementation.


namespace Npp_Plugin_Interface {	//  <---  Namespace --->

//*********************************************************************************************
//  Private interface implementation.

namespace {						//  <--- Unnamed namespace for private variables --->

//*********************************************************************************************
//  Private variables

//  <--- Npp Data --->
tstring _Name;					//  Name passed to NPP for use in the 'Plugins' menu.
HANDLE _hModule;				//  Notepad++'s handle for this plugin module.
HWND _hNpp;						//  Handle to Notepad++ for messaging.
HWND _hScintillaMain;			//  Handle to Notepad++'s main view for messaging.
HWND _hScintillaSecond;			//  Handle to Notepad++'s second view for messaging.
HWND _hCurrView;				//  Handle to the currently active Notepad++ view for messaging.
bool _bRefreshCurrViewH;		//  Indicates if hCurrView handle needs updating.
bool _isReady;					//  Flag if Notepad++ has finished initalizing.

//  <--- Function item vectors --->
std::vector<FuncItem> _PluginFuncVector;	//  Container for this plugin's function commands.

//*********************************************************************************************
//  Private functions

#ifdef UNICODE 
extern "C" __declspec(dllexport) BOOL isUnicode() 
{

	//  Notepad++ comes in two flavors, ansi and unicode, this plugin is made for the unicode version.
	return TRUE;

}
#endif	//  End: UNICODE


extern "C" __declspec(dllexport) void setInfo(NppData NppData)
{
	/*
	 *
	 *  Since these values should not be changed during runtime they are stored in
	 *  this 'private' area with public functions providing access to them.
	 *
	 */

	//PluginsManager forwards Notepad++'s handle information for use in messaging.

	_ASSERT(!_hNpp);		//  Assert that these handles haven't already been set.

	_hNpp = NppData._nppHandle;
	_hScintillaMain = NppData._scintillaMainHandle;
	_hScintillaSecond = NppData._scintillaSecondHandle;

}




HWND updateCurrViewHandle()				//  Updates handle pointer to the currently active view.
{

	int currentEdit;

	::SendMessage(_hNpp, NPPM_GETCURRENTSCINTILLA, 0, (LPARAM)&currentEdit);

	_bRefreshCurrViewH = false;

	return ( currentEdit == 0 ) ? ( _hScintillaMain ): ( _hScintillaSecond );

}


}  // End: Unnamed namespace for private variables

//*********************************************************************************************
//  Publicly defined functions.


void init(tstring name, HANDLE hModule)	//  Initialize Plugin.
{

	/*
	 *  A plugin is really a singleton from the point of view of the plugin so control over
	 *  initialization and variable updates is required, this should happen in your sources'
	 *  DllMain function.
	 *
	 */

	_ASSERT( _Name.empty() );			//  Assert that these values haven't already been set.
	_ASSERT( _Name.size() <= nbChar );	//  Ensure this plugin stays within name size limits.
							
	_Name.assign(name);
	_hModule = hModule;

}


void setPluginFuncItem(tstring Name, PFUNCPLUGINCMD pFunction,
											 int cmdID, bool init2Check, ShortcutKey* pShKey)
{
		/*
		 *  This function stores Function Items to the FuncItem vector.
		 *  
		 *  If three basic entries 'separator', 'help', and 'about' are stored as PluginFuncItems
		 *  and an extension to this namespace provides another FuncItems vector, the two can
		 *  be merged while at the same time ensuring the main plugins menu functions are at
		 *  the bottom (or top) of the plugin's menu in Npp.
		 *  
		 */

	// Notify if length is too long.
	if ( !( Name.length() < nbChar ) )
		::MessageBox(Npp_Plugin_Interface::hNpp(),
					TEXT("Function name is too long and will be truncated."),
					TEXT("Function Item Name Alert"),
					MB_ICONINFORMATION);

	FuncItem thisFunction;

	Name.copy(thisFunction._itemName, Name.length());
	thisFunction._itemName[ Name.length() ] = '\0';

	thisFunction._cmdID = cmdID;
	thisFunction._pFunc = pFunction;
	thisFunction._init2Check = init2Check;
	thisFunction._pShKey = pShKey;

	_PluginFuncVector.push_back(thisFunction);

}


HANDLE hModule()					//  Plugin module's handle.
{

	return _hModule;

}

HWND hNpp()							//  Returns the main Notepad++ handle.
{

	return _hNpp;

}

/*
 *
 *  The hMainView and hSecondView sometimes need to be used instead of the hCurrView when
 *  a message must reach both.  For instance a lexer plugin that works with highlights
 *  (indicators) and makes a change to the highlighter's value will only have it updated in
 *  the currently focused view unless the message is sent to both.
 *
 */

//  Returns handle used in messaging Notepad++'s main view.
HWND hMainView()
{ 

	return _hScintillaMain;

}

//  Returns handle used in messaging Notepad++'s secondary view.
HWND hSecondView()
{

	return _hScintillaSecond;

}

//  Returns handle for the currently focused Notepad++ view.
HWND hCurrView()
{
	/*
	 *
	 *  When messages need to be sent a handle must be provided, this handle points
	 *  to the View that is currently in focus.
	 *  The bRefreshCurrViewH will force it to be updated during run-time.  This can have lots
	 *  of uses; for example, a lexer plugin may need to verify the language of a buffer after
	 *  activation hCurrView() will ensure the message is sent to the correct handle.
	 *
	 */

	if ( _bRefreshCurrViewH || !_isReady ) {
		_hCurrView = updateCurrViewHandle();
	}

	return _hCurrView;

}

//  Returns the current target of hCurrView().  0 for main view and 1 for second view.
int intCurrView()
{
	return ( ( hCurrView() == hMainView() ) ? ( 0 ) : ( 1 ) );
}

void hCurrViewNeedsUpdate()
{
	_bRefreshCurrViewH = true;
}

//  Returns Notepad++'s initialization state.
bool isNppReady()
{
	/*
	 *
	 *  The NppReady flag can be used to control a plugins activites during Npp startup.
	 *  The hCurrView() forces on update of the handle on each message until this is set.
	 *
	 */

	return _isReady;

}

// The NppReady flag can be used to control a plugins activites during Npp startup.
// The hCurrView() function forces an update of the handle on each message processed
//  until this is set.
void setNppReady()
{

	_isReady = true;

}


//  Provides a response to PluginsManager's getFuncArray call and allows for extensions
//  to work with generating an array to send back that includes their own FuncItems.
FuncItem * getPluginFuncArray()
{

	return ( &_PluginFuncVector[0] );

}

//  Provides a response for a namespace's extension to retrieve and work with the plugin's
//  function item vector.  This is useful in merging and ordering function items together
//  before responding to the Npp plugin manager's getFuncArray() call.
//
//	  <---  See the NppExtLexerInterface for an example --->
//
std::vector<FuncItem> getPluginFuncVector()
{

	return ( _PluginFuncVector );

}

//  Provides a response to PluginsManager's getFuncArray call and allows for extensions
//  to work with generating an array to send back that includes their own FuncItems.
int getPluginFuncCount()
{

	return _PluginFuncVector.size();

}


//*********************************************************************************************
//  Exported Functions
								/*
								 *  These exported functions facilitate registering this plugin
								 *  into the Notepad++ Plugins Manager and the Scintilla
								 *  Lexer Library.
								 *  
								 */


//---------------------------------------------------------------------------------------------
//  Plugin Module Details - for Notepad++'s PluginsManager

//  PluginsManager retrieves the 'module name' for it's internal use in messaging
//  and function pointer control.
extern "C" __declspec(dllexport) const TCHAR * getName()
{

	return &_Name[0];

}



}  //  End namespace: Npp_Plugin_Interface

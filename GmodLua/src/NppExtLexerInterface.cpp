// NppExtLexerInterface.cpp
// This file is part of GmodLua.
// Written for Notepad++

// Copyright (C)2008-2009 Kyle Fleming ( garthex@gmail.com )

// The following code was written by Thell Fowler for the Notepad++ External Lexers Plugin
// and modified slightly for use with GmodLua.

///////////////////////////////////////////////////////////////////////////////////////////////

// NppExtLexerInterface.cpp

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
//                  Public Plugin Interface Extension Functions
//				   for use with Notepad++ External Lexer Plugins
///////////////////////////////////////////////////////////////////////////////////////////////

	/***
	 *
	 *  This file implements the extension to NppPluginInterface for using Scintilla's
	 *  external lexer capabilities in a Notepad++ plugin.
	 *
	 *  The commenting in this code may seem excessive, yet it is there for people who would
	 *  like to better understand how the plugin system works and how it can be used.
	 *
	 ***/

///////////////////////////////////////////////////////////////////////////////////////////////
//  Include Directives

#include "NppExtLexerInterface.h"
//#include "NppExtLexer_Common.h"		//  Included for access to lexer object creation.

///////////////////////////////////////////////////////////////////////////////////////////////
//  External Lexer Interface Implementation

namespace Npp_ExtLexer_Interface {

namespace {						//  <--- Unnamed namespace for private variables --->

//*********************************************************************************************
//  Private Interface Implementation

//---------------------------------------------------------------------------------------------
//  Parameter constants

//  <--- From Npp's Parameters.h
const int NB_MAX_EXTERNAL_LANG = 30;			
const int MAX_EXTERNAL_LEXER_NAME_LEN = 16;
const int MAX_EXTERNAL_LEXER_DESC_LEN = 32;

//  <--- Just for a little clarity in the function calls --->
const bool FOLD = true;
const bool LEX = false;

// <--- Containers --->
std::vector<FuncItem> _LexerFuncVector;	//  Container for lexer function commands.
std::vector<FuncItem> ReturnVector;		//  Container for the ordered function commands.
std::vector<Lexer> _LexerDetailVector;  //  Container for lexer details.


//*********************************************************************************************
//  Exported Lexer Functions.

//---------------------------------------------------------------------------------------------
//  Plugin Lexer Details - for Notepad++'s PluginsManager and Scintilla's LexerManager

				/*
				 *  Both managers need to know the details of lexers included in this plugin.
				 *
				 *  Notepad++ fills in the language menu and the style preferences dialog, as
				 *  well as handles style properties control with Scintilla.
				 *
				 *  Scintilla does all of the accessing of the document, application of styles,
				 *  folding, and so on using the information provided by Notepad++ and this
				 *  plugin's messaging.
				 *
				 */

int __stdcall GetLexerCount()
{

	return _LexerDetailVector.size();

}

int getLexerFuncCount ()
{

	return _LexerFuncVector.size();

}

void __stdcall GetLexerName(unsigned int Index, char *name, int buflength)
{

	//  Both Notepad++ and Scintilla use this.  Scintilla expects char and Notepad++ will
	//  convert it itself.

	std::string currLangName = _LexerDetailVector.at(Index)._name;

	if (buflength > 0 ) {
		buflength--;
		int n = currLangName.length();
		if (n > buflength) n = buflength;

		memcpy(name, currLangName.c_str(), n), name[n] = '\0';
	}

}


void __stdcall GetLexerStatusText(unsigned int Index, TCHAR *desc, int buflength)
{

	//  Notepad++ uses this, but Scintilla does not.  Which allows use of TCHAR vs char.

	tstring currLangDesc = _LexerDetailVector.at(Index)._description;;

	if (buflength > 0) {
		buflength--;
		int n = currLangDesc.length();
		if (n > buflength) n = buflength;

		TMEMCPY(desc, currLangDesc.c_str(), n), desc[n] = '\0';

	}

}


//---------------------------------------------------------------------------------------------
//  Scintilla Lexer Functions
					
				/*
				 *  The Fold and Lex functions are simple call relayers to a particular lexer
				 *  in this plugin.
				 *
				 */

void __stdcall Lex(unsigned int langID, unsigned int startPos, int length, int initStyle,
				   char *words[], WindowID window, char *props)
{
		/*
		 *  This is just a forwarding function that forwards Scintilla's fold command to
		 *  the correct exteneral lexer.
		 *
		 */

	namespace pIface = Npp_Plugin_Interface;

	Lexer* currLexer = &_LexerDetailVector.at(langID);


	if (! ( currLexer->SCI_LEXERID >= SCLEX_AUTOMATIC ) || ( pIface::isNppReady() ) ) {
		//  Each lexer has a LEXERID that Scintilla assigns.  For external lexers this ID isn't
		//  known until Scintilla's first call for a lexer to lex a document.
		//  When that happens the ID is stored and used for filtering notification messages.
		int lexerID = ::SendMessage(pIface::hCurrView(), SCI_GETLEXER, 0, 0);
		currLexer->SCI_LEXERID = lexerID;
	}
	else if ( currLexer->SCI_LEXERID < SCLEX_AUTOMATIC ) {
		
		::MessageBox(pIface::hNpp(), TEXT(" SCI_LEXERID is less than SCLEX_AUTOMATIC and Npp reports ready! " ),
			TEXT(" Npp External Lexer Problem "), MB_OK | MB_ICONEXCLAMATION );
		return;	// This shouldn't happen!
	}

	currLexer->_pLexOrFold(0, startPos, length, initStyle, words, window, props);

}

void __stdcall Fold(int langID, unsigned int startPos, int length,	int initStyle,
					char *words[], WindowID window, char *props)
{
		/*
		 *  This is just a forwarding function that forwards Scintilla's fold command to
		 *  the correct exteneral lexer.
		 *
		 */

		Lexer* currLexer = &_LexerDetailVector.at(langID);

		currLexer->_pLexOrFold(1, startPos, length, initStyle, words, window, props);

}



}  //  End:: Unnamed namespace for private implementation.

//*********************************************************************************************
//  Publicly defined functions.

void init(std::string Name, tstring statusText, NppExtLexerFunction pLexOrFold,
			   PFUNCPLUGINCMD pMenuDlg)						//  Setup a lexer definition.
{
	/*
	 *  This function initializes the lexer values that will get passed to both the Scintilla
	 *  External Lexer Library and Notepad++'s Plugin Manager.
	 *
	 */

	// Notify if length is too long.
	if ( Name.length() > MAX_EXTERNAL_LEXER_NAME_LEN )
		::MessageBox(Npp_Plugin_Interface::hNpp(),
					TEXT("Lexer name is too long and will be truncated."),
					TEXT("Lexer Name Alert"),
					MB_ICONINFORMATION);

	if ( statusText.length() > MAX_EXTERNAL_LEXER_DESC_LEN )
		::MessageBox(Npp_Plugin_Interface::hNpp(),
					TEXT("Lexer description is too long and will be truncated."),
					TEXT("Lexer Description Alert"),
					MB_ICONINFORMATION);


	//  The lexer details vector is used to store the SCI lexerID and the pointer to the
	//  lexers entry point function ( usually LexOrFold() ).
	Lexer thisLexer;

	thisLexer._name.assign(Name);
	thisLexer._description.assign(statusText);
	thisLexer._pLexOrFold = pLexOrFold;
	thisLexer.SCI_LEXERID = NULL;

	_LexerDetailVector.push_back(thisLexer);


	/*
	 *  This plugin extension uses the lexer's name for the menu.  Since the lexer name is also
	 *  used by Notepad++ and Scintilla as a char* value and the FuncItem expects a TCHAR*
	 *  value we need to convert it prior to registering the FuncItem.
	 *
	 */

	//  Create a tstring and copy the standard character string into it.  Should work for both
	//  unicode and ansi.
	std::string about = std::string("About ");
	int len = Name.length() + about.length();
	tstring itemName = tstring(len, '\0');
	std::copy(about.begin(), about.end(), itemName.begin());
	std::copy(Name.begin(), Name.end(), itemName.begin() + about.length());

	setLexerFuncItem(itemName, pMenuDlg);

}

void setLexerFuncItem(tstring Name, PFUNCPLUGINCMD pFunction,
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

	_LexerFuncVector.push_back(thisFunction);

}

int getSCILexerIDByIndex(int index)
{

	/*
	 *  Returns the SCI_LEXERID at vector index.  Useful when dealing with notifications and
	 *  messaging.
	 *
	 */

	return ( _LexerDetailVector.at(index).SCI_LEXERID );

}

int getSCILexerIDByName( std::string name )
{

	/*
	 *  Returns the SCI_LEXERID from the vector index item with the matching name.  Useful
	 *  when dealing with notifications and messaging.
	 *
	 */

	std::vector<Lexer>::iterator currLexer = _LexerDetailVector.begin();

	for (currLexer; currLexer < _LexerDetailVector.end(); currLexer++ ) {
		if ( currLexer->_name.compare( name ) == 0 ) return currLexer->SCI_LEXERID;
	}

	return ( MININT );

}

std::vector<FuncItem> getLexerFuncVector()
{
	/*
	 *
	 *  Provides a response for a namespace's extension to retrieve and work with the
	 *  plugin's function item vector.
	 *
	 *  Useful to merge additional function items together before responding to the
	 *  Npp plugin manager's getFuncArray() call.
	 *
	 */

	return ( _LexerFuncVector );

}

std::vector<Lexer> getLexerDetailVector()
{
	/*
	 *
	 *  Provides a response for a namespace's extension to retrieve and work with the
	 *  plugin's function item vector.
	 *
	 *  Useful to merge additional function items together before responding to the
	 *  Npp plugin manager's getFuncArray() call.
	 *
	 */

	return ( _LexerDetailVector );

}

//---------------------------------------------------------------------------------------------
//  'Virtualized' base plugin FuncItem functions.

	/*
	 *  These functions will be used instead of the base plugin functions to allow for the
	 *  inclusion of a sorted lexer FuncItem list in the menu above the base plugins.
	 *
	 */

FuncItem * getPluginFuncArray()			//  Return this plugin's sorted FuncItem array.
{
	/*
	 *  In order to have the function items show up in the Notepad++ plugins menu with
	 *  the base plugins' function items at the bottom a new vector is created and the existing
	 *  vectors are copied into it with the lexer functions first.
	 *
	 */


	namespace pIface = Npp_Plugin_Interface;


	//  <--- Local function for sorting criteria predicate. --->
	struct sortByName
	{
	     bool operator()(FuncItem& f1, FuncItem& f2)
	     {
		      return ( _tcscmp(f1._itemName, f2._itemName) < 0 );
		 }
	};

	if ( _LexerFuncVector.empty() ) {

		//  Doesn't look like there are any lexer function items so just send the plugin's.
		ReturnVector = pIface::getPluginFuncVector();
	}
	else {

		//  Sort the lexer function items vector.
		std::sort(_LexerFuncVector.begin(), _LexerFuncVector.end(), sortByName());

		//  Copy it to the vector that will be returned to Notepad++'s PluginsManager.
		ReturnVector = _LexerFuncVector;

		//  Append the base plugin's function items (ie 'Help', 'About', etc...
		std::vector<FuncItem> tmpVector = pIface::getPluginFuncVector();
		std::copy(tmpVector.begin(), tmpVector.end(), std::back_inserter(ReturnVector));
	}

	return ( &ReturnVector[0] );

}

int getPluginFuncCount()
{

	/*
	 *
	 *  Provides a response to PluginsManager's getFuncArray call and allows for extensions
	 *  to work with generating an array to send back that includes their own FuncItems.
	 *
	 */

	return ( Npp_Plugin_Interface::getPluginFuncCount() + getLexerFuncCount() );

}


}  // End namespace:  Npp_ExtLexer_Interface.

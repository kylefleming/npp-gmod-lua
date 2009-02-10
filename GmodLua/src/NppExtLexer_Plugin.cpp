// NppExternalLexer_Plugin.cpp
// This file is part of GmodLua.
// Written for Notepad++

// Copyright (C)2008-2009 Kyle Fleming ( garthex@gmail.com )

// The following code was written by Thell Fowler for the Notepad++ External Lexers Plugin
// and modified slightly for use with GmodLua.

///////////////////////////////////////////////////////////////////////////////////////////////

// NppExternalLexer_Plugin.cpp

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
//                      Main Plugin Entry and Control
//              for use with Notepad++ External Lexers Plugin
///////////////////////////////////////////////////////////////////////////////////////////////

/***
*
*  This file contains the non-language specific implementations of the functions declarations
*  needed to implement an external lexer plugin for Notepad++.
*
*  For help on adding a new language please see the How-To.txt file.  Essentially, to add
*  a simple language definition:
*
*    >  Include your language's definition in the NppExternalLexers.xml language style file.
*    >  Create a language header and source file, wrapping your language in a namespace.  Don't
*       forget a menu function.  ( NppExtLexer_<language> if you like. )
*    >  Add  your file to this file's include statements.
*    >  If you want, add your namespace to the Environment Setup area.
*    >  Add your language to the initialization statement in DLLMAIN().
*    >  Enjoy.
*
***/

///////////////////////////////////////////////////////////////////////////////////////////////
//  Include Directives

#include "NppExtLexer_Plugin.h"


//  ===>  INCLUDE YOUR LEXER HEADER HERE
#include "NppExtLexer_GmodLua.h"

//---------------------------------------------------------------------------------------------
//  Plugin Environment Setup

	/*
	 *  The in the namespaces below the v_getfuncarray alias allows for emulation of a class
	 *  'virtual' function feature giving a plugin extension the ability to override the base
	 *  plugin's getfuncArray() function.
	 *
	 *  An example of this is how the NppExtLexerInterface uses this to alphabetize the lexer
	 *  menu FuncItems and then append the base plugin's FuncItem so that the developer doesn't
	 *  need to think about what order the lexer FuncItems are added.
	 *
	 */

//  <--- Namespace Aliases --->
namespace pIface = Npp_Plugin_Interface;			//  The base plugin interface.
namespace lIface = Npp_ExtLexer_Interface;			//  The external lexer extension to the base interface.
namespace plugin = NppExtLexer_Plugin;				//  The plugin where this lexer was initialized.
namespace v_getfuncarray = lIface;					//  The active function array controller.

//  ===> Include your lexer's Namespace Alias here.
namespace l_gmodlua = NppExtLexer_GmodLua;


///////////////////////////////////////////////////////////////////////////////////////////////
//  Required Plugin Interface Routines

BOOL APIENTRY DllMain(HANDLE hModule, DWORD reasonForCall, LPVOID /*lpReserved*/)
{
	/*
	 *  This is where language lexers get registered to the interface.
	 *
	 *  To add a new language you need (at the minimum):
	 *    
	 */

	switch (reasonForCall)
	{

	case DLL_PROCESS_ATTACH:

		// <--- Base initialization of the plugin object --->
		pIface::init(TEXT("Gmod 10 Lua Lexer"), hModule);

		// <--- Base menu function items setup --->
		pIface::setPluginFuncItem(TEXT(""), NULL);	//  A separator line.
		// pIface::setPluginFuncItem(TEXT("Help.txt"), plugin::Help_func);
		pIface::setPluginFuncItem(TEXT("About NppExternalLexers Plugin"), plugin::About_func);

		// <--- Base initialization of lexer values --->

		/*
		 *  The format is:
		 *    - Language name within double quotes.  Shown in the 'Language' menu.
		 *    - A description within a TEXT(" ") statement.  Shown in the status bar.
		 *    - The name of the LexOrFold function in your namespace.
		 *    - The name of the menu dialog function in your namespace.
		 *
		 */

		//  ===> Include your lexer's initialization statement here.
		lIface::init( "Gmod 10 Lua", TEXT("Garry's Mod 10 Lua source file"),
			l_gmodlua::LexOrFold, l_gmodlua::menuDlg );


		// <--- Additional Menu Function Items --->

		/*
		 *  You can also add additional menu function items by calling
		 *  lIface::setLexerFuncItem.
		 *
		 */

		//lIface::setLexerFuncItem( TEXT("Name"), NAMESPACE::FUNCTION );


		break;

	case DLL_PROCESS_DETACH:
/*
		//  Cleanup the PowerShell Lexer class object.
		l_powershell::PowerShell_Lexer* lexer;
		lexer = l_powershell::getLexerObj();
		lexer->~PowerShell_Lexer();
*/
		break;

	case DLL_THREAD_ATTACH:
		break;

	case DLL_THREAD_DETACH:
		break;

	}

	return TRUE;

}


extern "C" __declspec(dllexport) FuncItem * getFuncsArray(int *FuncsArraySize)
{

	/*
	 *  PluginsManager stores pointers to the functions that are exposed, to
	 *  allow for dialog and menu interaction.  The base plugin's functions to retrieve
	 *  the size and array can be 'virtualized' by altering the namespace aliases located in
	 *  the 'Plugin environment setup' area of this file.
	 *
	 */

	*FuncsArraySize = v_getfuncarray::getPluginFuncCount();

	return ( v_getfuncarray::getPluginFuncArray() );

}


//---------------------------------------------------------------------------------------------
//  Notepad++ and Scintilla Communications Processing
				
	/*
	 *  Listings of notifications and messages are in Notepad_plus_msgs.h and Scintilla.h.
	 *
	 *  Notifications use the SCNotification structure described in Scintilla.h.
	 *
	 */


extern "C" __declspec(dllexport) void beNotified(SCNotification *notifyCode)
{
	/*
	 *  This function gives access to Notepad++'s notification facilities including forwarded
	 *  notifications from Scintilla.
	 *  
	 *  Notifications can be filtered and language specific handlers called using a
	 *  Namespace::Function() call.
	 *
	 *  To filter a notification to your specific lexer use the lIface::getSCILexerIDByName()
	 *  function and compare that to a value returned from messageProc(SCI_GETLEXER, 0, 0).
	 *
	 */

	int currSCILEXERID;		//  External lexers are assigned SCLEX_AUTOMATIC + id by Scintilla.


	// ===> Include optional notification handlers in the switch.

	switch (notifyCode->nmhdr.code) 
	{
	case SCN_MODIFIED:
		pIface::hCurrViewNeedsUpdate();
/*
		if (notifyCode->modificationType & (SC_MOD_DELETETEXT | SC_MOD_INSERTTEXT)) {
			currSCILEXERID = messageProc(SCI_GETLEXER, 0, 0);

			if ( currSCILEXERID > SCLEX_AUTOMATIC ) {

				if ( currSCILEXERID == lIface::getSCILexerIDByName("PowerShell*") ) {
					l_powershell::setDocModified( true );
				}
				
			}

		}
*/
		break;

	case NPPN_READY:
		pIface::setNppReady();
		pIface::hCurrViewNeedsUpdate();
/*
		currSCILEXERID = messageProc(SCI_GETLEXER, 0, 0);

		if ( currSCILEXERID > SCLEX_AUTOMATIC ) {

			if ( currSCILEXERID == lIface::getSCILexerIDByName("PowerShell*") ) {
				//  Highlighters don't get applied correctly until Npp is ready.
				messageProc(SCI_STARTSTYLING, -1, 0);
			}
			
		}
*/
		break;

	case NPPN_WORDSTYLESUPDATED:

		/*
		 *  You can use this notification to make sure that style configuration changes to
		 *  highlighter styles take effect without requiring the user to make a doc change.
		 *  Another use is to keep wordlists in memory while your language is active for the
		 *  focused buffer.
		 *
		 *  To see an example of both of these ideas in action see NppExtLexer_PowerShell.
		 *
		 */

		pIface::hCurrViewNeedsUpdate();
/*
		currSCILEXERID = messageProc(SCI_GETLEXER, 0, 0);

		if ( currSCILEXERID > SCLEX_AUTOMATIC ) {

			if ( currSCILEXERID == lIface::getSCILexerIDByName("PowerShell*") ) {
				l_powershell::WORDSTYLESUPDATEDproc();
			}
			
		}
*/
		break;

	case NPPN_LANGCHANGED:
		pIface::hCurrViewNeedsUpdate();
/*
		currSCILEXERID = messageProc(SCI_GETLEXER, 0, 0);

		if ( currSCILEXERID > SCLEX_AUTOMATIC ) {

			if ( currSCILEXERID == lIface::getSCILexerIDByName("PowerShell*") ) {
				l_powershell::setLanguageChanged( true );
			}
			
		}
*/
		break;

	case NPPN_BUFFERACTIVATED:
		pIface::hCurrViewNeedsUpdate();
/*
		currSCILEXERID = messageProc(SCI_GETLEXER, 0, 0);

		if ( currSCILEXERID > SCLEX_AUTOMATIC ) {

			if ( currSCILEXERID == lIface::getSCILexerIDByName("PowerShell*") ) {
				//  Make sure highlighters get applied to the whole doc.
				l_powershell::setLanguageChanged( true );  // This flags for a full doc lexing.
				messageProc(SCI_STARTSTYLING, -1, 0);
			}
			
		}
*/
		break;

	case NPPN_FILEOPENED:
		pIface::hCurrViewNeedsUpdate();
		break;

	default:
		break;

	}

}


extern "C" __declspec(dllexport) LRESULT messageProc(UINT Message, WPARAM wParam, LPARAM lParam)
{

	/*
	 *  This function give access to Notepad++'s messaging facilities.  It was originally
	 *  intended for the relay of Notepad++ messages and inter-plugin messages, yet having
	 *    ::SendMessage( hCurrentView, SOME_MESSAGE_TOKENNAME, value, value );
	 *  mixed in all over the place was ugly so this plugin uses messageProc to keep all of
	 *  these in one area.
	 *
	 *  This function either returns true or the return value that comes from each individual
	 *  handler.  So be sure to explicitly state the return in the switch case.
	 *
	 *  Use the pIface:: hCurrView, hMainView, hSecondView, and hNpp for the standard handles,
	 *  some messages needs to be sent to both the main and second handle to get the desired
	 *  results.  ( Indicator setup messages are one example. )
	 *
	 *  See Notepad_plus_msgs.h and Scintilla.h for notification IDs.
	 *
	 */

	
	using namespace pIface;

	// ===>  Include optional messaging handlers here.
	switch (Message)
	{
		//  It would be good to check if Notepad++'s second view actually needs messaging
		//  although for now it seems that sending some messages to both views has less overhead.

		//  Notepad++ Messages
		case NPPM_ACTIVATEDOC:
			SendMessage( hNpp(), NPPM_ACTIVATEDOC, wParam, lParam);
			break;
		case NPPM_GETCURRENTDOCINDEX:
			return SendMessage( hNpp(), NPPM_GETCURRENTDOCINDEX, wParam, lParam);

		//  Lexer messages
		case SCI_STARTSTYLING:
			SendMessage(hCurrView(), SCI_STARTSTYLING, wParam, lParam);
			break;
		case SCI_GETLEXER:
			return SendMessage(hCurrView(), SCI_GETLEXER, wParam, lParam);
		case SCI_BRACEMATCH:
			return SendMessage(hCurrView(), SCI_BRACEMATCH, wParam, lParam);

		// Style messages
		case SCI_STYLEGETBACK:
			return SendMessage(hCurrView(), SCI_STYLEGETBACK, wParam, lParam);
		case SCI_STYLEGETBOLD:
			return SendMessage(hCurrView(), SCI_STYLEGETBOLD, wParam, lParam);
		case SCI_STYLEGETITALIC:
			return SendMessage(hCurrView(), SCI_STYLEGETITALIC, wParam, lParam);
		case SCI_STYLEGETUNDERLINE:
			return SendMessage(hCurrView(), SCI_STYLEGETUNDERLINE, wParam, lParam);

		// Indicator messages
		// We send INDIC set messages to both handles to ensure style changes are properly set.
		case SCI_INDICSETSTYLE:
			SendMessage(hMainView(), SCI_INDICSETSTYLE, wParam, lParam);
			SendMessage(hSecondView(), SCI_INDICSETSTYLE, wParam, lParam);
			break;
		case SCI_INDICSETFORE:
			SendMessage(hMainView(), SCI_INDICSETFORE, wParam, lParam);
			SendMessage(hSecondView(), SCI_INDICSETFORE, wParam, lParam);
			break;
		case SCI_INDICGETFORE:
			return SendMessage(hCurrView(), SCI_INDICGETFORE, wParam, lParam);
		case SCI_INDICSETUNDER:
			SendMessage(hMainView(), SCI_INDICSETUNDER, wParam, lParam);
			SendMessage(hSecondView(), SCI_INDICSETUNDER, wParam, lParam);
			break;
		case SCI_INDICGETUNDER:
			return SendMessage(hCurrView(), SCI_INDICGETUNDER, wParam, lParam);
		case SCI_INDICSETALPHA:
			SendMessage(hMainView(), SCI_INDICSETALPHA, wParam, lParam);
			SendMessage(hSecondView(), SCI_INDICSETALPHA, wParam, lParam);
			break;

		default:
			return false;

	}  // End: Switch ( Message )

	return TRUE;

}


///////////////////////////////////////////////////////////////////////////////////////////////
//  Plugin Helper Functions

/*
void NppExtLexer_Plugin::Help_func()
{

	TCHAR directoryPath[MAX_PATH];
	::SendMessage( pIface::hNpp(), NPPM_GETNPPDIRECTORY, MAX_PATH, (LPARAM)directoryPath );
	
	tstring pluginHelpFile;
	pluginHelpFile.append(directoryPath);
	pluginHelpFile.append( TEXT("\\Plugins\\Config\\NppExtLexer_PluginHelp.txt") );

	//  The following will be an NPPM_DOOPEN, but that isn't implemented yet.

	::SendMessage( pIface::hNpp(), 3032, 0, (LPARAM)pluginHelpFile.c_str() );

	::MessageBox(pIface::hNpp(),
		TEXT("Make your way through this file to learn more about\n")
		TEXT("developing your own Notepad++ External Lexer."),
		TEXT("Notepad++ External Lexer Plugin Help Information"),
		MB_OK);

}*/

void NppExtLexer_Plugin::About_func() 
{

	::MessageBox(pIface::hNpp(),
		TEXT("  N++ External Lexers plugin was created to make it extremely easy to create\n")
		TEXT("  your own external lexer plugins to use with Notepad++.  For those who are new\n")
		TEXT("  to writing lexers it provides some helpful examples and lots of notes/comments.\n")
		TEXT("  For people who already use several external lexers and continously tinker around\n")
		TEXT("  making more, this plugin streamlines the interface and makes for quicker and\n")
		TEXT("  cleaner coding.\r\n")
		TEXT("  It is a pretty complete rewrite of the MyLexerTemplate by Kyle Fleming. \r\n\r\n")
		TEXT("  Huge thanks and credit go to the following for being kind enough to share their\n")
		TEXT("  time and knowledge by releasing sources to their projects:\r\n\r\n")
		TEXT("	Don Ho - for Notepad++.\r\n")
		TEXT("	Neil Hodgson - for Scintilla.\r\n")
		TEXT("	Jens Lorenz - for the great example plugins.\r\n")
		TEXT("	Kyle Fleming - for the Notepad++ external lexer work.\r\n")
		TEXT("	Freenode ##C++ and ##C++-Offtopic regulars for the patience.\r\n\r\n")
		TEXT("  Hopefully you find this plugin to be a useful tool.  Enjoy!\r\n")
		TEXT("  Thell Fowler (almostautomated)"),
		TEXT("About This Plugin"), MB_OK);
	
}



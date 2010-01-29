// GmodLua.cpp
// This file is part of GmodLua.
// Written for Notepad++

// Copyright (C)2008-2010 Kyle Fleming ( garthex@gmail.com )

// Code borrowed from the Notepad++ External Lexers Plugin.
// Copyright 2008 - 2009 Thell Fowler (thell@almostautomated.com)

// GmodLua is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// GmodLua is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with GmodLua.  If not, see <http://www.gnu.org/licenses/>.


#ifndef NPPEXTLEXER_GMODLUA_H
#define NPPEXTLEXER_GMODLUA_H

///////////////////////////////////////////////////////////////////////////////////////////////
//  Include Directives

#include "NppExtLexer_Common.h"		//  Provides all of the interface, messaging definitions,
									//  and namespace aliases.


///////////////////////////////////////////////////////////////////////////////////////////////
// Language Specific definitions

namespace NppExtLexer_GmodLua {

//*********************************************************************************************
//  Global ( within the namespace ) constants.

	
//*********************************************************************************************
// Nested namespace to place only the style state IDs
namespace Styles {

const int DEFAULT = 0;

const int LUA_COMMENT = 1;
const int CPP_COMMENT = 2;
const int LUA_COMMENTLINE = 3;
const int CPP_COMMENTLINE = 4;

const int NUMBER = 5;
const int OPERATOR = 6;
const int IDENTIFIER = 7;

const int STRING = 8;
const int CHARACTER = 9;
const int STRINGEOL = 10;
const int LITERALSTRING = 11;

const int WORD0 = 20;
const int WORD1 = 21;
const int WORD2 = 22;
const int WORD3 = 23;
const int WORD4 = 24;
const int WORD5 = 25;
const int WORD6 = 26;
const int WORD7 = 27;
const int WORD8 = 28;

};  // End namespace Style

//*********************************************************************************************
//  Lexer Function Delarations.

	/*
	 *  Place any special class, structs, lexer specific function declarations here.
	 *
	 */


//---------------------------------------------------------------------------------------------
//  Generic Externally called functions.

//  These are the functions registered in the ExtLexer_Plugin's DLLMAIN routine.

//  This is the entry point called by the external lexer interface.
void LexOrFold(bool LexorFold, unsigned int startPos, int length, int initStyle,
                  char *words[], WindowID window, char *props);

//  This is the menu dialog function item that shows up in the Notepad++ 'Plugins' menu under
//  your lexer's name.
void menuDlg();


}  //  End:  namespace NppExtLexer_GmodLua.

#endif  //  End: Include guard.
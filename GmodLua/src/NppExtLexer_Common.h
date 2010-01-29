// NppExtLexer_Common.h
// This file is part of GmodLua.
// Written for Notepad++

// Copyright (C)2008-2010 Kyle Fleming ( garthex@gmail.com )

// The following code has been modified from the original version written by Thell Fowler
// for the Notepad++ External Lexers Plugin.

///////////////////////////////////////////////////////////////////////////////////////////////

#ifndef NPPEXTLEXER_COMMON_H
#define NPPEXTLEXER_COMMON_H

// NppExtLexer_Common.h

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
//            Common includes that each added language will need
//              for use with Notepad++ External Lexers Plugin
///////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////
//  Required includes needed for Lexer plugin definitions.

/*
 *  See the lower part of this document for includes that are not required but are included
 *  because they add functionality that benefits lexers.
 *
 */

//---------------------------------------------------------------------------------------------
//  Standard library includes, using C++ headers instead of C.

#include <cstdlib>
										/*
										 *  Standard Lib
										 *  Included here to met requirements for PropSet.h
										 *
										 */


//---------------------------------------------------------------------------------------------
//  Plugin interface includes.

#include "NppPluginInterface.h"
#include "NppExtLexerInterface.h"
#include "NppExtLexer_Plugin.h"			
										/*
										 *  These provide everything that an external lexer
										 *  requires to register a lexer and communicate
										 *  with Notepad++ and Scintilla.
										 *  Includes:
										 *    # "PluginInterface.h"
										 *    # "Accessor.h"
										 *
										 */


//---------------------------------------------------------------------------------------------
//  Scintilla specific includes.

#include "Accessor.h"
										/*
										 *  Defines the interface for document buffer access.
										 *  No additional includes come from this file.
										 *
										 */


#include "PropSet.h"
										/*
										 *  Scintilla class definitions
										 *  Includes:
										 *    # "SString.h" -> String container/buffer access.
										 *
										 *  Required for Window Accessor
										 *
										 */

#include "PropSetSimple.h"


#include "WindowAccessor.h"				/*
										 *  Scintilla Accessor derived class
										 *  Required for External Lexer
										 *  No additional includes come from this file.
										 *
										 */


#include "KeyWords.h"
										/*
										 *  Scintilla definitions enabling LexerModule
										 *  Required for ExternalLexer
										 *  No additional includes come from this file.
										 *
										 */


#include "ExternalLexer.h"
										/*
										 *  Scintilla External Lexer classes
										 *  Required for external lexer control and access
										 *  No additional includes come from this file.
										 */ 

#include "StyleContext.h"
										/*
										 *  Scintilla Styling
										 *  Not really required but all the newer lexers use
										 *    the style context accessor interface.
										 *  No additional includes come from this file.
										 *
										 */

///////////////////////////////////////////////////////////////////////////////////////////////
//  Additional Includes

#include "CharacterSet.h"
										/*
										 *  Scintilla CharacterSet Class
										 *  Encapsulates a set of characters.
										 *  Used to test if a character is within a set.
										 *
										 */


///////////////////////////////////////////////////////////////////////////////////////////////
//  Common Namespace Aliases

namespace pIface = Npp_Plugin_Interface;	//  The base plugin interface.
namespace lIface = Npp_ExtLexer_Interface;	//  The external lexer extension to the base interface.
namespace plugin = NppExtLexer_Plugin;		//  The plugin where this lexer was initialized.


#endif  //  NPPEXTLEXER_COMMON_H

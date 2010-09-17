// This file is part of GmodLua.
// A plugin written for Notepad++

// Copyright (C)2008-2010 Kyle Fleming ( garthex@gmail.com )

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

#ifndef GMOD_LUA_H
#define GMOD_LUA_H

// Notepad++ includes
#include "precompiledHeaders.h"
#include "PluginInterface.h"

// Scintilla includes
#include "ILexer.h"
#include "PropSetSimple.h"
#include "WordList.h"
#include "LexerBase.h"

#include "LexAccessor.h"
#include "Accessor.h"
#include "LexerModule.h"
#include "StyleContext.h"
#include "CharacterSet.h"

#ifdef UNICODE
	#define generic_strncpy_s wcsncpy_s
#else
	#define generic_strncpy_s strncpy_s
#endif

namespace GmodLua
{
	static const generic_string PLUGIN_NAME = TEXT("&Gmod Lua Lexer");
	static const std::string LEXER_NAME = "Gmod Lua";
	static const generic_string LEXER_STATUS_TEXT = TEXT("Garry's Mod Lua source file");

	static const int numPluginMenuItems = 1;
	FuncItem pluginMenuItems[numPluginMenuItems];
	static const generic_string aboutMenuItem = TEXT("&About...");
	void aboutDlg();

	NppData nppData;

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

	class LexerGmodLua : public LexerBase {
	public:
		LexerGmodLua() {}

		void SCI_METHOD Lex(unsigned int startPos, int length, int initStyle, IDocument *pAccess);
		void SCI_METHOD Fold(unsigned int startPos, int length, int initStyle, IDocument *pAccess);

		static ILexer *LexerFactory() {
			try {
				return new LexerGmodLua();
			} catch (...) {
				// Should not throw into caller as may be compiled with different compiler or options
				return 0;
			}
		}
	};
	
	void Colourise_Doc(unsigned int startPos, int length, int initStyle, WordList *keywordlists[], Accessor &styler);
	void Fold_Doc(unsigned int startPos, int length, int initStyle, Accessor &styler);
};

#endif

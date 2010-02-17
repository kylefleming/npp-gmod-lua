// NppExtLexer_GmodLua.cpp
// This file is part of GmodLua.
// Written for Notepad++

// Copyright (C)2008-2010 Kyle Fleming ( garthex@gmail.com )

// Code borrowed from the Notepad++ External Lexers Plugin.
// Copyright 2008 - 2009 Thell Fowler (thell@almostautomated.com)

// Code borrowed from npp.4.6.src\scintilla\src\LexCaml.cxx
// Copyright 2005 by Robert Roessler <robertr@rftp.com>

// Code borrowed from npp.4.6.src\scintilla\src\LexLua.cxx
// LexLua.cxx written by Paul Winwood.
// LexLua.cxx folder by Alexey Yutkin.
// LexLua.cxx modified by Marcos E. Wurzius & Philippe Lhoste

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


///////////////////////////////////////////////////////////////////////////////////////////////
//  Include Directives

#include "NppExtLexer_GmodLua.h"	//  Provides the lexer specific definitions.


///////////////////////////////////////////////////////////////////////////////////////////////
//  <--- Namespace Aliases --->
namespace sID = NppExtLexer_GmodLua::Styles;		//  All of the style state codes.
namespace lex = NppExtLexer_GmodLua;				//  This lexer's namespace.


namespace NppExtLexer_GmodLua {

///////////////////////////////////////////////////////////////////////////////////////////////
//  Main Lexer Functions


// Extended to accept accented characters
static inline bool IsAWordChar(int ch) {
	return ch >= 0x80 ||
	       (isalnum(ch) || ch == '_');
}

static inline bool IsLuaOperator(int ch) {
	if (ch >= 0x80 || isalnum(ch)) {
		return false;
	}

	if (ch == '*' || ch == '/' || ch == '-' || ch == '+' ||
		ch == '(' || ch == ')' || ch == '=' ||
		ch == '{' || ch == '}' || ch == '~' ||
		/*ch == '[' ||*/ ch == ']' || ch == ';' ||
		ch == '<' || ch == '>' || ch == ',' ||
		/*ch == '.' ||*/ ch == '^' || ch == '%' || //ch == ':' ||
		ch == '#' || ch == '&' || ch == '|' ||
		ch == '!') {
		return true;
	}
	return false;
}

// Test for [=[ ... ]=] delimiters, returns 0 if it's only a [ or ],
// return 1 for [[ or ]], returns >=2 for [=[ or ]=] and so on.
// The maximum number of '=' characters allowed is 254.
static int LongDelimCheck(StyleContext &sc) {
	int sep = 1;
	while (sc.GetRelative(sep) == '=' && sep < 0xFF)
		sep++;
	if (sc.GetRelative(sep) == sc.ch)
		return sep;
	return 0;
}


//  <---  Colourise --->
void Colourise_Doc(unsigned int startPos, int length, int initStyle, WordList *keywordlists[], Accessor &styler)
{

	WordList &keywords0 = *keywordlists[0];
	WordList &keywords1 = *keywordlists[1];
	WordList &keywords2 = *keywordlists[2];
	WordList &keywords3 = *keywordlists[3];
	WordList &keywords4 = *keywordlists[4];
	WordList &keywords5 = *keywordlists[5];
	WordList &keywords6 = *keywordlists[6];
	WordList &keywords7 = *keywordlists[7];
	WordList &keywords8 = *keywordlists[8];

	int currentLine = styler.GetLine(startPos);
	int sepCount = 0;
	if (initStyle == sID::LITERALSTRING || initStyle == sID::LUA_COMMENT) {
		sepCount = styler.GetLineState(currentLine - 1) & 0xFF;
	}

	// Do not leak onto next line
	if (initStyle == sID::STRINGEOL || initStyle == sID::LUA_COMMENTLINE || initStyle == sID::CPP_COMMENTLINE) {
		initStyle = sID::DEFAULT;
	}

	StyleContext sc(startPos, length, initStyle, styler);
	for (; sc.More(); sc.Forward()) {
		if (sc.atLineEnd) {
			// Update the line state, so it can be seen by next line
			currentLine = styler.GetLine(sc.currentPos);
			switch (sc.state) {
			case sID::LITERALSTRING:
			case sID::LUA_COMMENT:
				// Inside a literal string or block comment, we set the line state
				styler.SetLineState(currentLine, sepCount);
				break;
			default:
				// Reset the line state
				styler.SetLineState(currentLine, 0);
				break;
			}
		}

		// Handle string line continuation
		if ((sc.state == sID::STRING || sc.state == sID::CHARACTER) &&
		        sc.ch == '\\') {
			if (sc.chNext == '\n' || sc.chNext == '\r') {
				sc.Forward();
				if (sc.ch == '\r' && sc.chNext == '\n') {
					sc.Forward();
				}
				continue;
			}
		}

		// Determine if the current state should terminate.
		if (sc.state == sID::OPERATOR || sc.state == sID::NUMBER) {
			sc.SetState(sID::DEFAULT);
		} else if (sc.state == sID::IDENTIFIER) {
			if (!IsAWordChar(sc.ch) || (sc.currentPos+1 == startPos + length)) {
				if (IsAWordChar(sc.ch)) {
					sc.Forward(); // Checks words at the end of the document.
				}
				char s[100];
				sc.GetCurrent(s, sizeof(s));
				if (keywords0.InList(s)) {
					sc.ChangeState(sID::WORD0);
				} else if (keywords1.InList(s)) {
					sc.ChangeState(sID::WORD1);
				} else if (keywords2.InList(s)) {
					sc.ChangeState(sID::WORD2);
				} else if (keywords5.InList(s)) {
					sc.ChangeState(sID::WORD5);
				} else if (keywords8.InList(s)) {
					sc.ChangeState(sID::WORD8);
				}
				sc.SetState(sID::DEFAULT);
			}
		} else if (sc.state == sID::LUA_COMMENTLINE || sc.state == sID::CPP_COMMENTLINE) {
			if (sc.atLineEnd) {
				sc.ForwardSetState(sID::DEFAULT);
			}
		} else if (sc.state == sID::STRING) {
			if (sc.ch == '\\') {
				if (sc.chNext == '\"' || sc.chNext == '\\') {
					sc.Forward();
				}
			} else if (sc.ch == '\"') {
				sc.ForwardSetState(sID::DEFAULT);
			} else if (sc.atLineEnd) {
				sc.ChangeState(sID::STRINGEOL);
				sc.ForwardSetState(sID::DEFAULT);
			}
		} else if (sc.state == sID::CHARACTER) {
			if (sc.ch == '\\') {
				if (sc.chNext == '\'' || sc.chNext == '\\') {
					sc.Forward();
				}
			} else if (sc.ch == '\'') {
				sc.ForwardSetState(sID::DEFAULT);
			} else if (sc.atLineEnd) {
				sc.ChangeState(sID::STRINGEOL);
				sc.ForwardSetState(sID::DEFAULT);
			}
		} else if (sc.state == sID::LITERALSTRING || sc.state == sID::LUA_COMMENT) {
			if (sc.ch == ']') {
				int sep = LongDelimCheck(sc);
				if (sep == sepCount) {
					sc.Forward(sep);
					sc.ForwardSetState(sID::DEFAULT);
				}
			}
		} else if (sc.state == sID::CPP_COMMENT) {
			if (sc.Match('*', '/')) {
				sc.Forward();
				sc.ForwardSetState(sID::DEFAULT);
			}
		} else {
			sc.SetState(sID::DEFAULT);
		}

		// Determine if a new state should be entered.
		if (sc.state == sID::DEFAULT) {
			if (IsADigit(sc.ch) || (sc.ch == '.' && IsADigit(sc.chNext))) {
				sc.SetState(sID::NUMBER);
				if (sc.ch == '0' && (sc.chNext == 'x' || sc.chNext == 'X')) {
					int j = 1;
					while (IsADigit(sc.GetRelative(j+1), 16)) {
						j++;
					}
					sc.Forward(j);
				} else {
					int j = 0;
					bool hitDot(false);
					bool hitE(false);
					int ch = sc.ch;
					int chNext = sc.chNext;
					while (IsADigit(ch) || (ch == '.' && !hitDot && !hitE) ||
							((ch == 'e' || ch == 'E') && !hitE && (IsADigit(chNext) ||
							((chNext == '-' || chNext == '+') && IsADigit(sc.GetRelative(j+2)))))) {
						if (ch == '.') {
							hitDot = true;
						} else if (ch == 'e' || ch == 'E') {
							hitE = true;
							j++;
							if (chNext == '-' || chNext == '+') {
								j++;
							}
							chNext = sc.GetRelative(j+1);
						}
						j++;
						ch = chNext;
						chNext = sc.GetRelative(j+1);
					}
					sc.Forward(j-1);
				}
			} else if (sc.ch == ':' || sc.ch == '.') {
				sc.SetState(sID::OPERATOR);
				if (IsAWordChar(sc.chNext) || isspace(sc.chNext)) {
					int j = 1;
					int ch = sc.chNext;
					while((sc.currentPos + j < startPos + length) && isspace(ch)) {
						ch = sc.GetRelative(++j);
					}
					if (IsAWordChar(ch)) {
						int i = 0;
						char s[101];
						while (i < 100 && IsAWordChar(ch)) {
							s[i++] = ch;
							ch = sc.GetRelative(++j);
						}
						s[i++] = '\0';
						if (keywords7.InList(s)) {
							sc.Forward(j-i+1);
							sc.SetState(sID::WORD7);
						}
						if (sc.state != sID::OPERATOR) {
							sc.Forward(i-2);
						}
					}
				}
			} else if (IsAWordChar(sc.ch)) {
				sc.SetState(sID::IDENTIFIER);
				int j = 0;
				int i = 0;
				char s[101];
				bool finished = false;
				int ch = sc.ch;

				while (i < 100 && (sc.currentPos + j < startPos + length) && !finished &&
						(IsAWordChar(ch) || isspace(ch) || (ch == '.' || ch == ':'))) {
					if (!IsAWordChar(ch)) {
						while((sc.currentPos + j < startPos + length) && isspace(ch)) {
							ch = sc.GetRelative(++j);
						}
						if (i < 100 && (ch == '.' || ch == ':')) {
							s[i++] = ch;
							ch = sc.GetRelative(++j);
							while((sc.currentPos + j < startPos + length) && isspace(ch)) {
								ch = sc.GetRelative(++j);
							}
							while(i < 100 && IsAWordChar(ch)) {
								s[i++] = ch;
								ch = sc.GetRelative(++j);
							}
							s[i++] = '\0';
							if (keywords3.InList(s)) {
								sc.ChangeState(sID::WORD3);
							} else if (keywords4.InList(s)) {
								sc.ChangeState(sID::WORD4);
							} else if (keywords6.InList(s)) {
								sc.ChangeState(sID::WORD6);
							}
							if (sc.state != sID::IDENTIFIER) {
								sc.Forward(j-1);
							}
						}

						finished = true;
					} else {
						s[i++] = ch;
						ch = sc.GetRelative(++j);
					}
				}
			} else if (sc.ch == '\"') {
				sc.SetState(sID::STRING);
			} else if (sc.ch == '\'') {
				sc.SetState(sID::CHARACTER);
			} else if (sc.ch == '[') {
				sepCount = LongDelimCheck(sc);
				if (sepCount == 0) {
					sc.SetState(sID::OPERATOR);
				} else {
					sc.SetState(sID::LITERALSTRING);
					sc.Forward(sepCount);
				}
			} else if (sc.Match('-', '-')) {
				sc.SetState(sID::LUA_COMMENTLINE);
				if (sc.Match("--[")) {
					sc.Forward(2);
					sepCount = LongDelimCheck(sc);
					if (sepCount > 0) {
						sc.ChangeState(sID::LUA_COMMENT);
						sc.Forward(sepCount);
					}
				} else {
					sc.Forward();
				}
			} else if (sc.Match('/', '/')) {
				sc.SetState(sID::CPP_COMMENTLINE);
				sc.Forward();
			} else if (sc.Match('/', '*')) {
				sc.SetState(sID::CPP_COMMENT);
				sc.Forward();
			} else if (IsLuaOperator(sc.ch)) {
				sc.SetState(sID::OPERATOR);
			}
		}
	}
	sc.Complete();
}

//  <--- Fold --->
void Fold_Doc(unsigned int startPos, int length, int initStyle, WordList *[], Accessor &styler)
{
	unsigned int lengthDoc = startPos + length;
	int visibleChars = 0;
	int lineCurrent = styler.GetLine(startPos);
	int levelPrev = styler.LevelAt(lineCurrent) & SC_FOLDLEVELNUMBERMASK;
	int levelCurrent = levelPrev;
	char chNext = styler[startPos];
	bool foldCompact = styler.GetPropertyInt("fold.compact", 1) != 0;
	int styleNext = styler.StyleAt(startPos);
	int style = initStyle;
	char s[10];

	for (unsigned int i = startPos; i < lengthDoc; i++) {
		char ch = chNext;
		chNext = styler.SafeGetCharAt(i + 1);
		int stylePrev = style;
		style = styleNext;
		styleNext = styler.StyleAt(i + 1);
		bool atEOL = (ch == '\r' && chNext != '\n') || (ch == '\n');
		if (style == sID::WORD0 && stylePrev != sID::WORD0) {
			if (ch == 'i' || ch == 'd' || ch == 'f' || ch == 'e' || ch == 'r' || ch == 'u') {
				unsigned int j;
				for (j = 0; j < 8 && iswordchar(styler.SafeGetCharAt(i + j)); j++) {
					s[j] = styler[i + j];
				}
				s[j] = '\0';

				if ((strcmp(s, "if") == 0) || (strcmp(s, "do") == 0) || (strcmp(s, "function") == 0) || (strcmp(s, "repeat") == 0)) {
					levelCurrent++;
				}
				if ((strcmp(s, "end") == 0) || (strcmp(s, "until") == 0)) {
					levelCurrent--;
				}
			}
		} else if (style == sID::OPERATOR) {
			if (ch == '{'){
				levelCurrent++;
			} else if (ch == '}'){
				levelCurrent--;
			}
		} else if ((style == sID::LITERALSTRING || style == sID::LUA_COMMENT || style == sID::CPP_COMMENT) &&
				!(stylePrev == sID::LITERALSTRING || stylePrev == sID::LUA_COMMENT || stylePrev == sID::CPP_COMMENT) &&
				(ch == '[' || ch == '/' || ch == '-')) {
			levelCurrent++;
		} else if ((style == sID::LITERALSTRING || style == sID::LUA_COMMENT || style == sID::CPP_COMMENT) &&
				!(styleNext == sID::LITERALSTRING || styleNext == sID::LUA_COMMENT || styleNext == sID::CPP_COMMENT) &&
				(ch == ']' || ch == '/')) {
			levelCurrent--;
		} else if (style == sID::CPP_COMMENTLINE || style == sID::LUA_COMMENTLINE) {
			if ((ch == '/' && chNext == '/') || (ch == '-' && chNext == '-')) {
				char chNext2 = styler.SafeGetCharAt(i + 2);
				if (chNext2 == '{') {
					levelCurrent++;
				} else if (chNext2 == '}') {
					levelCurrent--;
				}
			}
		}

		if (!isspacechar(ch)) {
			visibleChars++;
		}
		if (atEOL || (i == lengthDoc-1)) {
			int lev = levelPrev;
			if (visibleChars == 0 && foldCompact) {
				lev |= SC_FOLDLEVELWHITEFLAG;
			}
			if (levelCurrent > levelPrev) {
				lev |= SC_FOLDLEVELHEADERFLAG;
			}
			if (lev != styler.LevelAt(lineCurrent)) {
				styler.SetLevel(lineCurrent, lev);
			}
			lineCurrent++;
			levelPrev = levelCurrent;
			visibleChars = 0;
		}
	}
	char lastChar = styler.SafeGetCharAt(lengthDoc-1);
	if ((unsigned)styler.Length() == lengthDoc && (lastChar == '\n' || lastChar == '\r')) {
		styler.SetLevel(lineCurrent, levelCurrent);
	}
}


///////////////////////////////////////////////////////////////////////////////////////////////
//  Entry Point Functions.

//  Scintilla lexer entry point.
void LexOrFold(bool foldOrLex, unsigned int startPos, int length, int initStyle,
                  char *words[], WindowID window, char *props)
{

	// Create and initialize the WindowAccessor (including contained PropSet)
	PropSetSimple ps;
	ps.SetMultiple(props);
	WindowAccessor wa(window, ps);

	//  Create and initialize WordList(s).
	//  If you have an extremely large word file, or lots of styling rules you may want to speed
	//  up processing by storing the wordlists instead of reprocessing them on each call.
	int nWL = 0;
	for (; words[nWL]; nWL++) ;	// count # of WordList PTRs needed
	WordList** wl = new WordList* [nWL + 1];// alloc WordList PTRs
	int i = 0;
	for (; i < nWL; i++) {
		wl[i] = new WordList();	// (works or THROWS bad_alloc EXCEPTION)
		wl[i]->Set(words[i]);
	}
	wl[i] = 0;


	// Set the currView handle to update at least once per lexer call.
	pIface::hCurrViewNeedsUpdate();


	//  Call the internal folding and styling functions.
	// foldOrLex is false for lex and true for fold
	if (foldOrLex) {

		// This is a nice helpful routine to back up a line to fix broken folds.
		int lineCurrent = wa.GetLine(startPos);
		if (lineCurrent > 0) {
			lineCurrent--;
			int newStartPos = wa.LineStart(lineCurrent);
			length += startPos - newStartPos;
			startPos = newStartPos;
			initStyle = 0;
			if (startPos > 0) {
				initStyle = wa.StyleAt(startPos - 1);
			}
		}

		Fold_Doc(startPos, length, initStyle, wl, wa);

	}
	else {

		//  You may want to put a routine here to backtrack past leaking styles, typically
		//  multiline styles, or just put such logic in the Colour_Doc function itself.  Just
		//  be sure to do it prior to creating your Style Context.

		Colourise_Doc(startPos, length, initStyle, wl, wa);

	}

	//  The flush function is what actually finalizes settings the styles you just coloured.
	wa.Flush();

	// Clean up the wordlists before leaving.
	for (i = nWL - 1; i >= 0; i--)
		delete wl[i];
	delete [] wl;

}



//  Notepad++ dialog entry point.
void menuDlg()
{

	::MessageBox(pIface::hNpp(),
		TEXT("Garry's Mod 10 Lua Syntax Highlighter 1.4\r\n")
		TEXT("http://code.google.com/p/npp-gmod-lua/\r\n\r\n")
		TEXT("                   Author: Kyle Fleming\n                          (aka Garthex)"),
		TEXT("<- About ->"),
		MB_OK);

}

}  //  End:  namespace NppExtLexer_GmodLua.

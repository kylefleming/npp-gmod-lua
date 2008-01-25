// GmodLua.cpp
// This file is part of GmodLua.
// Written for Notepad++

// Copyright (C)2008 Kyle Fleming ( garthex@gmail.com )

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

#include "GmodLua.h"

const char PLUGIN_NAME[] = "&Gmod Lua";
const char LEXER_NAME[] = "Gmod 10 Lua";
const char LEXER_DESC[] = "Garry's Mod 10 Lua source file";
const int nbFunc = 1;

HANDLE g_hModule = NULL;
NppData nppData;
FuncItem funcItem[nbFunc];


BOOL APIENTRY DllMain(HANDLE hModule, DWORD reasonForCall, LPVOID lpReserved)
{
	g_hModule = hModule;

	switch (reasonForCall)
	{
		case DLL_PROCESS_ATTACH:
		{
			/* Set function pointers */
			funcItem[0]._pFunc = aboutDlg;
		    	
			/* Fill menu names */
			strncpy(funcItem[0]._itemName, "&About...", sizeof("&About..."));

			/* Set shortcuts */
			funcItem[0]._pShKey = NULL;
			
			break;
		}	
		case DLL_PROCESS_DETACH:
			break;

		case DLL_THREAD_ATTACH:
			break;
			
		case DLL_THREAD_DETACH:
			break;
	}

	return TRUE;
}

extern "C" __declspec(dllexport) void setInfo(NppData notpadPlusData)
{
	nppData = notpadPlusData;
}

extern "C" __declspec(dllexport) const char * getName()
{
	return PLUGIN_NAME;
}

extern "C" __declspec(dllexport) FuncItem * getFuncsArray(int *nbF)
{
	*nbF = nbFunc;
	return funcItem;
}

extern "C" __declspec(dllexport) void beNotified(SCNotification *notifyCode)
{
	switch (notifyCode->nmhdr.code) 
	{
	case NPPN_READY:
		break;
	}
}

// Here you can process the Npp Messages 
// I will make the messages accessible little by little, according to the need of plugin development.
// Please let me know if you need to access to some messages :
// http://sourceforge.net/forum/forum.php?forum_id=482781
//
extern "C" __declspec(dllexport) LRESULT messageProc(UINT Message, WPARAM wParam, LPARAM lParam)
{
	return TRUE;
}

void aboutDlg(){
	::MessageBox(nppData._nppHandle, "Garry's Mod 10 Lua Syntax Highlighter 1.1\nhttp://code.google.com/p/npp-gmod-lua/\n\n                      Author: Garthex", "<-About->", MB_OK);
}

static void ColouriseGmodLuaDoc( unsigned int startPos, int length,
	int initStyle, WordList *keywordlists[], Accessor &styler);

static void FoldGmodLuaDoc(unsigned int startPos, int length,
	int initStyle, WordList *keywordlists[], Accessor &styler);

static void InternalLexOrFold(int lexOrFold, unsigned int startPos, int length,
	int initStyle, char *words[], WindowID window, char *props);

void EXT_LEXER_DECL Fold(unsigned int lexer, unsigned int startPos, int length,
	int initStyle, char *words[], WindowID window, char *props)
{
	if(lexer == 0)
		InternalLexOrFold(1, startPos, length, initStyle, words, window, props);
}

void EXT_LEXER_DECL Lex(unsigned int lexer, unsigned int startPos, int length,
	int initStyle, char *words[], WindowID window, char *props)
{
	if(lexer == 0)
		InternalLexOrFold(0, startPos, length, initStyle, words, window, props);
}

static void InternalLexOrFold(int foldOrLex, unsigned int startPos, int length,
	int initStyle, char *words[], WindowID window, char *props){

	// create and initialize a WindowAccessor (including contained PropSet)
	PropSet ps;
	ps.SetMultiple(props);
	WindowAccessor wa(window, ps);
	// create and initialize WordList(s)
	int nWL = 0;
	for (; words[nWL]; nWL++) ;	// count # of WordList PTRs needed
	WordList** wl = new WordList* [nWL + 1];// alloc WordList PTRs
	int i = 0;
	for (; i < nWL; i++) {
		wl[i] = new WordList();	// (works or THROWS bad_alloc EXCEPTION)
		wl[i]->Set(words[i]);
	}
	wl[i] = 0;
	// call our "internal" folder/lexer (... then do Flush!)
	if (foldOrLex)
		FoldGmodLuaDoc(startPos, length, initStyle, wl, wa);
	else
		ColouriseGmodLuaDoc(startPos, length, initStyle, wl, wa);
	wa.Flush();
	// clean up before leaving
	for (i = nWL - 1; i >= 0; i--)
		delete wl[i];
	delete [] wl;
}

int EXT_LEXER_DECL GetLexerCount(){
	return 1;
}
void EXT_LEXER_DECL GetLexerName(unsigned int Index, char *name, int buflength){
   if (buflength > 0) {
      buflength--;
      int n = (int)strlen(LEXER_NAME);
      if (n > buflength)
         n = buflength;
      memcpy(name, LEXER_NAME, n), name[n] = '\0';
   }
}
void EXT_LEXER_DECL GetLexerStatusText(unsigned int Index, char *name, int buflength){
   if (buflength > 0) {
      buflength--;
      int n = (int)strlen(LEXER_DESC);
      if (n > buflength)
         n = buflength;
      memcpy(name, LEXER_DESC, n), name[n] = '\0';
   }
}

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

static void ColouriseGmodLuaDoc(unsigned int startPos, int length,
	int initStyle, WordList *keywordlists[], Accessor &styler) {

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
	if (initStyle == GMOD_STYLE_LITERALSTRING || initStyle == GMOD_STYLE_LUA_COMMENT) {
		sepCount = styler.GetLineState(currentLine - 1) & 0xFF;
	}

	// Do not leak onto next line
	if (initStyle == GMOD_STYLE_STRINGEOL || initStyle == GMOD_STYLE_LUA_COMMENTLINE || initStyle == GMOD_STYLE_CPP_COMMENTLINE) {
		initStyle = GMOD_STYLE_DEFAULT;
	}

	StyleContext sc(startPos, length, initStyle, styler);
	for (; sc.More(); sc.Forward()) {
		if (sc.atLineEnd) {
			// Update the line state, so it can be seen by next line
			currentLine = styler.GetLine(sc.currentPos);
			switch (sc.state) {
			case GMOD_STYLE_LITERALSTRING:
			case GMOD_STYLE_LUA_COMMENT:
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
		if ((sc.state == GMOD_STYLE_STRING || sc.state == GMOD_STYLE_CHARACTER) &&
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
		if (sc.state == GMOD_STYLE_OPERATOR || sc.state == GMOD_STYLE_NUMBER) {
			sc.SetState(GMOD_STYLE_DEFAULT);
		} else if (sc.state == GMOD_STYLE_IDENTIFIER) {
			if (!IsAWordChar(sc.ch) || (sc.currentPos+1 == startPos + length)) {
				if (IsAWordChar(sc.ch)) {
					sc.Forward(); // Checks words at the end of the document.
				}
				char s[100];
				sc.GetCurrent(s, sizeof(s));
				if (keywords0.InList(s)) {
					sc.ChangeState(GMOD_STYLE_WORD0);
				} else if (keywords1.InList(s)) {
					sc.ChangeState(GMOD_STYLE_WORD1);
				} else if (keywords2.InList(s)) {
					sc.ChangeState(GMOD_STYLE_WORD2);
				} else if (keywords5.InList(s)) {
					sc.ChangeState(GMOD_STYLE_WORD5);
				} else if (keywords8.InList(s)) {
					sc.ChangeState(GMOD_STYLE_WORD8);
				}
				sc.SetState(GMOD_STYLE_DEFAULT);
			}
		} else if (sc.state == GMOD_STYLE_LUA_COMMENTLINE || sc.state == GMOD_STYLE_CPP_COMMENTLINE) {
			if (sc.atLineEnd) {
				sc.ForwardSetState(GMOD_STYLE_DEFAULT);
			}
		} else if (sc.state == GMOD_STYLE_STRING) {
			if (sc.ch == '\\') {
				if (sc.chNext == '\"' || sc.chNext == '\\') {
					sc.Forward();
				}
			} else if (sc.ch == '\"') {
				sc.ForwardSetState(GMOD_STYLE_DEFAULT);
			} else if (sc.atLineEnd) {
				sc.ChangeState(GMOD_STYLE_STRINGEOL);
				sc.ForwardSetState(GMOD_STYLE_DEFAULT);
			}
		} else if (sc.state == GMOD_STYLE_CHARACTER) {
			if (sc.ch == '\\') {
				if (sc.chNext == '\'' || sc.chNext == '\\') {
					sc.Forward();
				}
			} else if (sc.ch == '\'') {
				sc.ForwardSetState(GMOD_STYLE_DEFAULT);
			} else if (sc.atLineEnd) {
				sc.ChangeState(GMOD_STYLE_STRINGEOL);
				sc.ForwardSetState(GMOD_STYLE_DEFAULT);
			}
		} else if (sc.state == GMOD_STYLE_LITERALSTRING || sc.state == GMOD_STYLE_LUA_COMMENT) {
			if (sc.ch == ']') {
				int sep = LongDelimCheck(sc);
				if (sep == sepCount) {
					sc.Forward(sep);
					sc.ForwardSetState(GMOD_STYLE_DEFAULT);
				}
			}
		} else if (sc.state == GMOD_STYLE_CPP_COMMENT) {
			if (sc.Match('*', '/')) {
				sc.Forward();
				sc.ForwardSetState(GMOD_STYLE_DEFAULT);
			}
		} else {
			sc.SetState(GMOD_STYLE_DEFAULT);
		}

		// Determine if a new state should be entered.
		if (sc.state == GMOD_STYLE_DEFAULT) {
			if (IsADigit(sc.ch) || (sc.ch == '.' && IsADigit(sc.chNext))) {
				sc.SetState(GMOD_STYLE_NUMBER);
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
				sc.SetState(GMOD_STYLE_OPERATOR);
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
							sc.SetState(GMOD_STYLE_WORD7);
						}
						if (sc.state != GMOD_STYLE_OPERATOR) {
							sc.Forward(i-2);
						}
					}
				}
			} else if (IsAWordChar(sc.ch)) {
				sc.SetState(GMOD_STYLE_IDENTIFIER);
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
								sc.ChangeState(GMOD_STYLE_WORD3);
							} else if (keywords4.InList(s)) {
								sc.ChangeState(GMOD_STYLE_WORD4);
							} else if (keywords6.InList(s)) {
								sc.ChangeState(GMOD_STYLE_WORD6);
							}
							if (sc.state != GMOD_STYLE_IDENTIFIER) {
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
				sc.SetState(GMOD_STYLE_STRING);
			} else if (sc.ch == '\'') {
				sc.SetState(GMOD_STYLE_CHARACTER);
			} else if (sc.ch == '[') {
				sepCount = LongDelimCheck(sc);
				if (sepCount == 0) {
					sc.SetState(GMOD_STYLE_OPERATOR);
				} else {
					sc.SetState(GMOD_STYLE_LITERALSTRING);
					sc.Forward(sepCount);
				}
			} else if (sc.Match('-', '-')) {
				sc.SetState(GMOD_STYLE_LUA_COMMENTLINE);
				if (sc.Match("--[")) {
					sc.Forward(2);
					sepCount = LongDelimCheck(sc);
					if (sepCount > 0) {
						sc.ChangeState(GMOD_STYLE_LUA_COMMENT);
						sc.Forward(sepCount);
					}
				} else {
					sc.Forward();
				}
			} else if (sc.Match('/', '/')) {
				sc.SetState(GMOD_STYLE_CPP_COMMENTLINE);
				sc.Forward();
			} else if (sc.Match('/', '*')) {
				sc.SetState(GMOD_STYLE_CPP_COMMENT);
				sc.Forward();
			} else if (IsLuaOperator(sc.ch)) {
				sc.SetState(GMOD_STYLE_OPERATOR);
			}
		}
	}
	sc.Complete();
}

static void FoldGmodLuaDoc(unsigned int startPos, int length, int initStyle, WordList *[],
                       Accessor &styler) {
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
		if (style == GMOD_STYLE_WORD0 && stylePrev != GMOD_STYLE_WORD0) {
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
		} else if (style == GMOD_STYLE_OPERATOR) {
			if (ch == '{'){
				levelCurrent++;
			} else if (ch == '}'){
				levelCurrent--;
			}
		} else if ((style == GMOD_STYLE_LITERALSTRING || style == GMOD_STYLE_LUA_COMMENT || style == GMOD_STYLE_CPP_COMMENT) &&
				!(stylePrev == GMOD_STYLE_LITERALSTRING || stylePrev == GMOD_STYLE_LUA_COMMENT || stylePrev == GMOD_STYLE_CPP_COMMENT) &&
				(ch == '[' || ch == '/' || ch == '-')) {
			levelCurrent++;
		} else if ((style == GMOD_STYLE_LITERALSTRING || style == GMOD_STYLE_LUA_COMMENT || style == GMOD_STYLE_CPP_COMMENT) &&
				!(styleNext == GMOD_STYLE_LITERALSTRING || styleNext == GMOD_STYLE_LUA_COMMENT || styleNext == GMOD_STYLE_CPP_COMMENT) &&
				(ch == ']' || ch == '/')) {
			levelCurrent--;
		} else if (style == GMOD_STYLE_CPP_COMMENTLINE || style == GMOD_STYLE_LUA_COMMENTLINE) {
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

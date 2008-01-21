// GmodLua.h
// This file is part of GmodLua.
// Written for Notepad++

// Copyright (C)2008 Kyle Fleming ( garthex@gmail.com )

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

#define WIN32_LEAN_AND_MEAN

#define GMOD_STYLE_DEFAULT 0

#define GMOD_STYLE_LUA_COMMENT 1
#define GMOD_STYLE_CPP_COMMENT 2
#define GMOD_STYLE_LUA_COMMENTLINE 3
#define GMOD_STYLE_CPP_COMMENTLINE 4

#define GMOD_STYLE_NUMBER 5
#define GMOD_STYLE_OPERATOR 6
#define GMOD_STYLE_IDENTIFIER 7

#define GMOD_STYLE_STRING 8
#define GMOD_STYLE_CHARACTER 9
#define GMOD_STYLE_STRINGEOL 10
#define GMOD_STYLE_LITERALSTRING 11

#define GMOD_STYLE_WORD0 20
#define GMOD_STYLE_WORD1 21
#define GMOD_STYLE_WORD2 22
#define GMOD_STYLE_WORD3 23
#define GMOD_STYLE_WORD4 24
#define GMOD_STYLE_WORD5 25
#define GMOD_STYLE_WORD6 26
#define GMOD_STYLE_WORD7 27
#define GMOD_STYLE_WORD8 28

#include <stdlib.h>

#include "PluginInterface.h"
#include "Platform.h"
#include "PropSet.h"
#include "Accessor.h"
#include "StyleContext.h"
#include "KeyWords.h"
#include "ExternalLexer.h"
#include "WindowAccessor.h"

void aboutDlg();

#endif

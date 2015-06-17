# Introduction #

With Gmod now using Lua 5.1.2, I decided to update the Notepad++ syntax highlighter I wrote awhile ago. This time I coordinated with Don HO, the admin of Notepad++, to release an update to N++ which I wrote that supports external lexers. This allows me to release this as a plugin instead of recompiling Scintilla each time. Now you can get updates for Notepad++ and still use the same plugin in the future.


# Change Log #
## Release 1.5: September 16, 2010 ##
  * Revamped plugin for Scintilla's new external lexer interface
  * Did some hacky stuff to keep backwards compatibility
  * Updated keyword list
  * Brought back ANSI version

## Release 1.4: February 17, 2010 ##
  * Updated keyword list
  * Changed name from Gmod 10 Lua to Gmod Lua

## Release 1.3: January 29, 2010 ##
  * Updated dependencies to reflect recent changes to Notepad++ headers.
  * Updated GmodLua.xml keyword list.
  * Modified comment font to match changes in Notepad++'s vanilla lua styles.

## Release 1.2: February 10, 2009 ##
  * Fixed the issue created when N++ switched to Unicode from ANSI.
  * Converted the plugin to the system created by Thell Fowler. (http://poshcode.org/notepad++lexer/)
  * Updated GmodLua.xml to reflect the most recent lua function set.
  * Removed the unoptimized and lite versions.

## Release 1.1: January 25, 2008 ##
  * Fixed keyword generator and updated keyword list to be consistent with the 1-15-08 Gmod patch.
  * Fixed folding so it doesn't fold past the last character except when folding the last line of the document and the last line is blank.
  * Removed folding parenthesis.

## Release 1.0: January 12, 2008 ##
  * Support for [==[ ]==] style comments and quotes as specified by Lua 5.1
  * Improved code to determine numbers.
  * The ability to set C++ and Lua comments as different styles.
  * An XML that is fully customizable to allow you to add up to 9 keyword categories for syntax highlighting.
  * Default keyword groups for instruction words, global constants, global functions, scripted variables, scripted functions, libraries, library calls, object functions, and red flags.
  * Fixed problems with the last line of a document.
  * More intelligent word highlighter:
    * Spaces between words and period such as file . Write
    * First checks if word1.word2 is a word
    * If it isn't, then it checks for word1 and word2 separately.
    * Allows a colon ( : ) in a word in addition to a period.


## Available from the Previous Implementation ##
  * Folding!
  * A line with either --{, --}, //{ or //} will also fold.
  * Support for both C++ and Lua comments and strings.
  * Customizable styles within the preferences dialog.
  * Escape Character '\' So you can escape the end of a line properly and escape quotes properly
  * Single-line strings when unclosed at the end of a line don't try and parse the rest of the document as a string.
/* 
**A simple library of console routines (from http://sunlightd.virtualave.net/Windows/FAQ.html) 
---------------------------------------
** Clrscr() - clear the screen, and reset cursor to (0, 0)
** Gotoxy(x, y) - move the text cursor to position (x, y)
** SelectBackColour(colour) - select a background colour from the colour constants list
** SelectTextColour(colour) - select a text colour from the colour constants list
** NOTE:Assume that whenever you write text to the screen the attributes will apply to all text
** sent to the screen from that point until the next point at which you select different text 
** or background colour, and output text with the new attributes.
*/
#ifndef ConsoleUtilsH
#define ConsoleUtilsH

#include <windows.h>

//colour constants for translation
const WORD clBlack = 0;
const WORD clDarkRed = 1;
const WORD clDarkGreen = 2;
const WORD clDarkBlue = 3;
const WORD clDarkCyan = 4;
const WORD clDarkMagenta = 5;
const WORD clDarkYellow = 6;
const WORD clDarkGrey = 7;
const WORD clGrey = 8;
const WORD clRed = 9;
const WORD clGreen = 10;
const WORD clBlue = 11;
const WORD clCyan = 12;
const WORD clMagenta = 13;
const WORD clYellow = 14;
const WORD clWhite = 15;

//-------------
//clear the screen, and reset cursor to (0, 0)
void Clrscr();

//move the text cursor to position (x, y)
void Gotoxy(int x, int y);

//select a background colour from the colour constants list
void SelectBackColour(int colour);

//select a text colour from the colour constants list
void SelectTextColour(int colour);

//select the text attributes
void SelectAttributes();

//get the heigth of the screen (console window)
int screenHeight();

//get the width of the screen (console window)
int screenWidth();

#endif
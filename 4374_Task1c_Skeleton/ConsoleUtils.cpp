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
#include "ConsoleUtils.h"

WORD backColour = 0;
WORD textColour = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;
WORD textAttributes = backColour | textColour;

//-------------
//clear the screen, and reset cursor to (0, 0)
void Clrscr()
{
	//from web-site
	COORD coordScreen = { 0, 0 }; 
	DWORD cCharsWritten; 
	CONSOLE_SCREEN_BUFFER_INFO csbi; 
	DWORD dwConSize; 
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE); 
	GetConsoleScreenBufferInfo(hConsole, &csbi); 
	dwConSize = csbi.dwSize.X * csbi.dwSize.Y; 
	FillConsoleOutputCharacter(hConsole, TEXT(' '), dwConSize, 
	coordScreen, &cCharsWritten); 
	GetConsoleScreenBufferInfo(hConsole, &csbi); 
	FillConsoleOutputAttribute(hConsole, csbi.wAttributes, dwConSize, 
	coordScreen, &cCharsWritten); 
	SetConsoleCursorPosition(hConsole, coordScreen); 
}

//-------------
//move the text cursor to position (x, y)
void Gotoxy(int x, int y)
{
	//from web-site
	COORD coord;
	coord.X = x;
	coord.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

//-------------
//select a background colour from the colour constants list
void SelectBackColour(int colour)
{
	void SelectAttributes();
	switch (colour)
	{
		case clBlack: backColour = 0; break;
		case clDarkRed: backColour = BACKGROUND_RED; break;
		case clDarkGreen: backColour = BACKGROUND_GREEN; break;
		case clDarkBlue: backColour = BACKGROUND_BLUE; break;
		case clDarkCyan: backColour = BACKGROUND_GREEN | BACKGROUND_BLUE; break;
		case clDarkMagenta: backColour = BACKGROUND_RED | BACKGROUND_BLUE; break;
		case clDarkYellow: backColour = BACKGROUND_RED | BACKGROUND_GREEN; break;
		case clDarkGrey: backColour = BACKGROUND_INTENSITY; break;
		case clGrey: backColour = BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE; break;
		case clRed: backColour = BACKGROUND_INTENSITY | BACKGROUND_RED; break;
		case clGreen: backColour = BACKGROUND_INTENSITY | BACKGROUND_GREEN; break;
		case clBlue: backColour = BACKGROUND_INTENSITY | BACKGROUND_BLUE; break;
		case clCyan: backColour = BACKGROUND_INTENSITY | BACKGROUND_GREEN | BACKGROUND_BLUE; break;
		case clMagenta: backColour = BACKGROUND_INTENSITY | BACKGROUND_RED | BACKGROUND_BLUE; break;
		case clYellow: backColour = BACKGROUND_INTENSITY | BACKGROUND_RED | BACKGROUND_GREEN; break;
		default: backColour = BACKGROUND_INTENSITY | BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE;
	}
	SelectAttributes();
}

//-------------
//select a text colour from the colour constants list
void SelectTextColour(int colour)
{
	void SelectAttributes();
	switch (colour)
	{
		case clBlack: textColour = 0; break;
		case clDarkRed: textColour = FOREGROUND_RED; break;
		case clDarkGreen: textColour = FOREGROUND_GREEN; break;
		case clDarkBlue: textColour = FOREGROUND_BLUE; break;
		case clDarkCyan: textColour = FOREGROUND_GREEN | FOREGROUND_BLUE; break;
		case clDarkMagenta: textColour = FOREGROUND_RED | FOREGROUND_BLUE; break;
		case clDarkYellow: textColour = FOREGROUND_RED | FOREGROUND_GREEN; break;
		case clDarkGrey: textColour = FOREGROUND_INTENSITY; break;
		case clGrey: textColour = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE; break;
		case clRed: textColour = FOREGROUND_INTENSITY | FOREGROUND_RED; break;
		case clGreen: textColour = FOREGROUND_INTENSITY | FOREGROUND_GREEN; break;
		case clBlue: textColour = FOREGROUND_INTENSITY | FOREGROUND_BLUE; break;
		case clCyan: textColour = FOREGROUND_INTENSITY | FOREGROUND_GREEN | FOREGROUND_BLUE; break;
		case clMagenta: textColour = FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_BLUE; break;
		case clYellow: textColour = FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN; break;
		default: textColour = FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;
	}
	SelectAttributes();
}

//-------------
//select the text attributes
void SelectAttributes()
{
	textAttributes = backColour | textColour;
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), textAttributes);
}

//-------------
//get the heigth of the screen (console window)
int screenHeight()
{
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
	return csbi.srWindow.Bottom + 1;
}

//-------------
//get the width of the screen (console window)
int screenWidth()
{
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
	return csbi.srWindow.Right + 1;
}



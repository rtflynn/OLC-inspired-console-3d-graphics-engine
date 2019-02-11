#include "olcConsoleGameEngine.h"	


CONSOLE_FONT_INFOEX setPixelFont(int fontw, int fonth);

CONSOLE_SCREEN_BUFFER_INFO buildScreenBuffer(int width, int height, HANDLE consoleHandle);

void SetScreenBufferSize(int width, int height, HANDLE consoleHandle);

Console::Console()
{
	m_nScreenWidth = 80;
	m_nScreenHeight = 30;

	m_hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	m_hConsoleIn = GetStdHandle(STD_INPUT_HANDLE);

	m_sAppName = L"Default Game Engine";
}

void Console::SetConsole(int width, int height, int fontw, int fonth)
{
	assert(m_hConsole != INVALID_HANDLE_VALUE);
	m_nScreenWidth = width;
	m_nScreenHeight = height;

	m_screenBufferCorners = { 0, 0, 1, 1 };
	SetConsoleWindowInfo(m_hConsole, TRUE, &m_screenBufferCorners);
	SetScreenBufferSize(width, height, m_hConsole);
	assert(SetConsoleActiveScreenBuffer(m_hConsole));
		
	CONSOLE_FONT_INFOEX cfi = setPixelFont(fontw, fonth);
	assert(SetCurrentConsoleFontEx(m_hConsole, false, &cfi));

	m_screenBufferCorners = { 0, 0, (short)m_nScreenWidth - 1, (short)m_nScreenHeight - 1 };		// Set physical console window size
	assert(SetConsoleWindowInfo(m_hConsole, TRUE, &m_screenBufferCorners));
	m_screenBuffer = new CHAR_INFO[m_nScreenWidth*m_nScreenHeight];
}





































CONSOLE_FONT_INFOEX setPixelFont(int fontw, int fonth)
{
	CONSOLE_FONT_INFOEX cfi;
	cfi.cbSize = sizeof(cfi);
	cfi.nFont = 0;
	cfi.dwFontSize.X = fontw;
	cfi.dwFontSize.Y = fonth;
	cfi.FontFamily = FF_DONTCARE;
	cfi.FontWeight = FW_NORMAL;
	wcscpy_s(cfi.FaceName, L"Consolas");			// Set font to Consolas
	return cfi;
}


CONSOLE_SCREEN_BUFFER_INFO buildScreenBuffer(int width, int height, HANDLE consoleHandle)
{
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	assert(GetConsoleScreenBufferInfo(consoleHandle, &csbi));		// Puts m_hConsole's screen buffer info into csbi
	assert(height <= csbi.dwMaximumWindowSize.Y);
	assert(width <= csbi.dwMaximumWindowSize.X);
	return csbi;
}						// Not used any more.  Delete once we're comfortable with this fact.


void SetScreenBufferSize(int width, int height, HANDLE consoleHandle)
{
	COORD coord = { (short)width, (short)height };
	assert(SetConsoleScreenBufferSize(consoleHandle, coord));
}

// Define our static variables
std::atomic<bool> Console::m_bAtomActive(false);
std::condition_variable Console::m_cvGameFinished;
std::mutex Console::m_muxGame;




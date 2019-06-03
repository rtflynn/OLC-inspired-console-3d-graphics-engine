/*
	The nuts and bolts of this engine were originally created by OneLoneCoder:

	https://www.github.com/onelonecoder
	https://www.onelonecoder.com
	https://www.youtube.com/javidx9

~~~~~~
Twitter: @javidx9	http://twitter.com/javidx9
Blog:				http://www.onelonecoder.com
YouTube:			http://www.youtube.com/javidx9
*/

/*
Input is handled for you - interrogate the m_keys[] array with the virtual
keycode you want to know about. bPressed is set for the frame the key is pressed down
in, bHeld is set if the key is held down, bReleased is set for the frame the key
is released in. The same applies to mouse! m_mousePosX and Y can be used to get
the current cursor position, and m_mouse[1..5] returns the mouse buttons.

The draw routines treat characters like pixels. By default they are set to white solid
blocks - but you can draw any unicode character, using any of the colours listed below.
*/

/*
More on keys:   The key codes are the Virtual Key Codes found at https://docs.microsoft.com/en-us/windows/desktop/inputdev/virtual-key-codes .
So, we might have a condition like:   

if (GetKey(0x25).bHeld) { do stuff;}

Here, 0x25 refers to the left arrow.
Options are bHeld, bPressed, bReleased.  Key codes range from 0 to 255.

*/


#ifndef OLCCONSOLEGAMEENGINE_H
#define OLCCONSOLEGAMEENGINE_H

#pragma comment(lib, "winmm.lib")					// For GetAsyncKeyState

#ifndef UNICODE
#error Please enable UNICODE for your compiler! VS: Project Properties -> General -> \
Character Set -> Use Unicode. Thanks! - Javidx9
#endif


#include <windows.h>
#include <iostream>
#include <chrono>
#include <vector>
#include <list>
#include <thread>
#include <atomic>
#include <condition_variable>
#include "pixel_enums.h"
#include "keyboard.h"
#include "mouse.h"
#include <assert.h>


class Console
{
protected:
	int m_nScreenWidth;
	int m_nScreenHeight;
	CHAR_INFO *m_screenBuffer;
	std::wstring m_sAppName;
	HANDLE m_hOriginalConsole;			// Return to this after destroying our game engine object, or if displaying an error.
	HANDLE m_hConsole;
	HANDLE m_hConsoleIn;
	SMALL_RECT m_screenBufferCorners;			

	static std::atomic<bool> m_bAtomActive;
	static std::condition_variable m_cvGameFinished;
	static std::mutex m_muxGame;

public:							
	Console();

	virtual bool OnUserCreate() = 0;
	virtual bool OnUserUpdate(float fElapsedTime) = 0;

	Keyboard m_keyboard = Keyboard();
	Mouse m_mouse = Mouse(m_hConsoleIn);

	void SetConsole(int width, int height, int fontw, int fonth);

	int ScreenWidth() { return m_nScreenWidth; }
	int ScreenHeight() { return m_nScreenHeight; }

	void Start();
	

protected:
	// We could move this to Drawer.h and Drawer.cpp, but I'd rather have this here *as the interface* 
	// between Drawer and Console.
	virtual void Draw(int x, int y, short character = 0x2588, short color = 0x000F)
	{
		if (x >= 0 && x < m_nScreenWidth && y >= 0 && y < m_nScreenHeight)
		{
			m_screenBuffer[(m_nScreenHeight - y - 1) * m_nScreenWidth + x].Char.UnicodeChar = character;
			m_screenBuffer[(m_nScreenHeight - y - 1) * m_nScreenWidth + x].Attributes = color;
		}
	}

public:

	~Console()
	{
		SetConsoleActiveScreenBuffer(m_hOriginalConsole);
		delete[] m_screenBuffer;
	}


private:
	void GameThread()
	{
		// Create user resources as part of this thread
		// This is only run ONCE --- in its own thread.  i.e. this is only ever run from the Console::Start() method.

		if (!OnUserCreate())			
			m_bAtomActive = false;

		auto tp1 = std::chrono::system_clock::now();
		auto tp2 = std::chrono::system_clock::now();

		while (m_bAtomActive)				
		{			
			// Handle Timing
			tp2 = std::chrono::system_clock::now();
			std::chrono::duration<float> elapsedTime = tp2 - tp1;
			tp1 = tp2;
			float fElapsedTime = elapsedTime.count();

			// Handle Keyboard Input
			m_keyboard.update();
			// Handle Mouse Input.  Currently not working with laptop trackpad.  FIX LATER!!!
			m_mouse.update();

			// Handle Frame Update
			if (!OnUserUpdate(fElapsedTime))
				m_bAtomActive = false;

			// Update Title & Present Screen Buffer
			wchar_t s[256];
			swprintf_s(s, 256, L"OneLoneCoder-Inspired Console Game Engine - %s - FPS: %3.2f", m_sAppName.c_str(), 1.0f / fElapsedTime);
			SetConsoleTitle(s);
			WriteConsoleOutput(m_hConsole, m_screenBuffer, { (short)m_nScreenWidth, (short)m_nScreenHeight }, { 0,0 }, &m_screenBufferCorners);
		}
	}


protected:
	static BOOL CloseHandler(DWORD evt)
	{
		// Note this gets called in a seperate OS thread, so it must
		// only exit when the game has finished cleaning up, or else
		// the process will be killed before OnUserDestroy() has finished
		if (evt == CTRL_CLOSE_EVENT)
		{
			m_bAtomActive = false;

			// Wait for thread to be exited

			std::unique_lock<std::mutex> tempLock(m_muxGame);
			m_cvGameFinished.wait(tempLock);
		}
		return true;
	}
};


#endif

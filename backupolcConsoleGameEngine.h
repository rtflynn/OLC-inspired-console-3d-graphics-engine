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







	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	//
	//		Okay, so:  The 'Draw' method is an 'atom' here, meaning all the other drawing methods call it to do anything at all.
	//		That is, we can use the 'Draw' method as an interface and have all our shape-drawing methods in another class
	//		that inherits from this one.  This will be a very fruitful refactoring.
	//
	//
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


	virtual void Draw(int x, int y, short character = 0x2588, short color = 0x000F)
	{
		if (x >= 0 && x < m_nScreenWidth && y >= 0 && y < m_nScreenHeight)
		{
			m_screenBuffer[y * m_nScreenWidth + x].Char.UnicodeChar = character;
			m_screenBuffer[y * m_nScreenWidth + x].Attributes = color;
		}
	}


	void Fill(int x1, int y1, int x2, int y2, short character = 0x2588, short color = 0x000F)
	{
		Clamp(x1, y1);
		Clamp(x2, y2);
		for (int x = x1; x < x2; x++)
			for (int y = y1; y < y2; y++)
				Draw(x, y, character, color);
	}

	void DrawString(int x, int y, std::wstring characters, short color = 0x000F)
	{
		for (size_t i = 0; i < characters.size(); i++)
		{
			m_screenBuffer[y * m_nScreenWidth + x + i].Char.UnicodeChar = characters[i];
			m_screenBuffer[y * m_nScreenWidth + x + i].Attributes = color;
		}
	}

	void DrawStringAlpha(int x, int y, std::wstring characters, short color = 0x000F)
	{
		for (size_t i = 0; i < characters.size(); i++)
		{
			if (characters[i] != L' ')
			{
				m_screenBuffer[y * m_nScreenWidth + x + i].Char.UnicodeChar = characters[i];
				m_screenBuffer[y * m_nScreenWidth + x + i].Attributes = color;
			}
		}
	}

	void Clamp(int &x, int &y)			// Should be called Clamp so we don't confuse it with the process of Clipping
	{
		if (x < 0) x = 0;
		if (x >= m_nScreenWidth) x = m_nScreenWidth;
		if (y < 0) y = 0;
		if (y >= m_nScreenHeight) y = m_nScreenHeight;
	}

	void DrawLine(int x1, int y1, int x2, int y2, short character = 0x2588, short color = 0x000F)
	{
		int x, y, dx, dy, dx1, dy1, px, py, xe, ye, i;
		dx = x2 - x1; dy = y2 - y1;
		dx1 = abs(dx); dy1 = abs(dy);
		px = 2 * dy1 - dx1;	py = 2 * dx1 - dy1;
		if (dy1 <= dx1)
		{
			if (dx >= 0)
				{ x = x1; y = y1; xe = x2; }
			else
				{ x = x2; y = y2; xe = x1;}

			Draw(x, y, character, color);
			
			for (i = 0; x<xe; i++)
			{
				x = x + 1;
				if (px<0)
					px = px + 2 * dy1;
				else
				{
					if ((dx<0 && dy<0) || (dx>0 && dy>0)) y = y + 1; else y = y - 1;
					px = px + 2 * (dy1 - dx1);
				}
				Draw(x, y, character, color);
			}
		}
		else
		{
			if (dy >= 0)
				{ x = x1; y = y1; ye = y2; }
			else
				{ x = x2; y = y2; ye = y1; }

			Draw(x, y, character, color);

			for (i = 0; y<ye; i++)
			{
				y = y + 1;
				if (py <= 0)
					py = py + 2 * dx1;
				else
				{
					if ((dx<0 && dy<0) || (dx>0 && dy>0)) x = x + 1; else x = x - 1;
					py = py + 2 * (dx1 - dy1);
				}
				Draw(x, y, character, color);
			}
		}
	}

	void DrawTriangle(int x1, int y1, int x2, int y2, int x3, int y3, short character = 0x2588, short color = 0x000F)
	{
		DrawLine(x1, y1, x2, y2, character, color);
		DrawLine(x2, y2, x3, y3, character, color);
		DrawLine(x3, y3, x1, y1, character, color);
	}

	// https://www.avrfreaks.net/sites/default/files/triangles.c
	void FillTriangle(int x1, int y1, int x2, int y2, int x3, int y3, short character = 0x2588, short color = 0x000F)
	{
		auto SWAP = [](int &x, int &y) { int t = x; x = y; y = t; };
		auto drawline = [&](int sx, int ex, int ny) { for (int i = sx; i <= ex; i++) Draw(i, ny, character, color); };
		
		int t1x, t2x, y, minx, maxx, t1xp, t2xp;
		bool changed1 = false;
		bool changed2 = false;
		int signx1, signx2, dx1, dy1, dx2, dy2;
		int e1, e2;
		// Sort vertices
		if (y1>y2) { SWAP(y1, y2); SWAP(x1, x2); }
		if (y1>y3) { SWAP(y1, y3); SWAP(x1, x3); }
		if (y2>y3) { SWAP(y2, y3); SWAP(x2, x3); }

		t1x = t2x = x1; y = y1;   // Starting points
		dx1 = (int)(x2 - x1); if (dx1<0) { dx1 = -dx1; signx1 = -1; }
		else signx1 = 1;
		dy1 = (int)(y2 - y1);

		dx2 = (int)(x3 - x1); if (dx2<0) { dx2 = -dx2; signx2 = -1; }
		else signx2 = 1;
		dy2 = (int)(y3 - y1);

		if (dy1 > dx1) {   // swap values
			SWAP(dx1, dy1);
			changed1 = true;
		}
		if (dy2 > dx2) {   // swap values
			SWAP(dy2, dx2);
			changed2 = true;
		}

		e2 = (int)(dx2 >> 1);
		// Flat top, just process the second half
		if (y1 == y2) goto next;
		e1 = (int)(dx1 >> 1);

		for (int i = 0; i < dx1;) {
			t1xp = 0; t2xp = 0;
			if (t1x<t2x) { minx = t1x; maxx = t2x; }
			else { minx = t2x; maxx = t1x; }
			// process first line until y value is about to change
			while (i<dx1) {
				i++;
				e1 += dy1;
				while (e1 >= dx1) {
					e1 -= dx1;
					if (changed1) t1xp = signx1;//t1x += signx1;
					else          goto next1;
				}
				if (changed1) break;
				else t1x += signx1;
			}
			// Move line
		next1:
			// process second line until y value is about to change
			while (1) {
				e2 += dy2;
				while (e2 >= dx2) {
					e2 -= dx2;
					if (changed2) t2xp = signx2;//t2x += signx2;
					else          goto next2;
				}
				if (changed2)     break;
				else              t2x += signx2;
			}
		next2:
			if (minx>t1x) minx = t1x; if (minx>t2x) minx = t2x;
			if (maxx<t1x) maxx = t1x; if (maxx<t2x) maxx = t2x;
			drawline(minx, maxx, y);    // Draw line from min to max points found on the y
										 // Now increase y
			if (!changed1) t1x += signx1;
			t1x += t1xp;
			if (!changed2) t2x += signx2;
			t2x += t2xp;
			y += 1;
			if (y == y2) break;

		}
	next:
		// Second half
		dx1 = (int)(x3 - x2); if (dx1<0) { dx1 = -dx1; signx1 = -1; }
		else signx1 = 1;
		dy1 = (int)(y3 - y2);
		t1x = x2;

		if (dy1 > dx1) {   // swap values
			SWAP(dy1, dx1);
			changed1 = true;
		}
		else changed1 = false;

		e1 = (int)(dx1 >> 1);

		for (int i = 0; i <= dx1; i++) {
			t1xp = 0; t2xp = 0;
			if (t1x<t2x) { minx = t1x; maxx = t2x; }
			else { minx = t2x; maxx = t1x; }
			// process first line until y value is about to change
			while (i<dx1) {
				e1 += dy1;
				while (e1 >= dx1) {
					e1 -= dx1;
					if (changed1) { t1xp = signx1; break; }//t1x += signx1;
					else          goto next3;
				}
				if (changed1) break;
				else   	   	  t1x += signx1;
				if (i<dx1) i++;
			}
		next3:
			// process second line until y value is about to change
			while (t2x != x3) {
				e2 += dy2;
				while (e2 >= dx2) {
					e2 -= dx2;
					if (changed2) t2xp = signx2;
					else          goto next4;
				}
				if (changed2)     break;
				else              t2x += signx2;
			}
		next4:

			if (minx>t1x) minx = t1x; if (minx>t2x) minx = t2x;
			if (maxx<t1x) maxx = t1x; if (maxx<t2x) maxx = t2x;
			drawline(minx, maxx, y);   										
			if (!changed1) t1x += signx1;
			t1x += t1xp;
			if (!changed2) t2x += signx2;
			t2x += t2xp;
			y += 1;
			if (y>y3) return;
		}
	}


	~Console()
	{
		SetConsoleActiveScreenBuffer(m_hOriginalConsole);
		delete[] m_screenBuffer;
	}






public:
	void Start()
	{	
		m_bAtomActive = true;														// Start the thread
		std::thread t = std::thread(&Console::GameThread, this);

		t.join();																	// Wait for thread to be exited
	}


private:
	void GameThread()
	{
		// Create user resources as part of this thread
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
			std::unique_lock<std::mutex> ul(m_muxGame);
			m_cvGameFinished.wait(ul);
		}
		return true;
	}


};




// Define our static variables
//std::atomic<bool> Console::m_bAtomActive(false);
//std::condition_variable Console::m_cvGameFinished;
//std::mutex Console::m_muxGame;


#endif


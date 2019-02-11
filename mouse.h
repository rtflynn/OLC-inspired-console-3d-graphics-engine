#ifndef MOUSE_H
#define MOUSE_H
#include "keystate.h"
#pragma comment(lib, "user32.lib")					// For GetAsyncKeyState



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// FIX LATER!!!  Mouse isn't working with my laptop trackpad --- at least, I think that's what's going on.
// Good thing is, we don't really need the mouse for anything soon.
// Good thing we separated it out anyway...
//  JUST DON'T FORGET TO FIX OR DELETE THIS FILE

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/*

struct KeyState
{
	bool bPressed = false;
	bool bReleased = false;
	bool bHeld = false;
};


bool isPressed(bool oldState, bool newState)
{
	return (oldState == 0) && (newState == 1);		// Should only return true for the frame in which the key is pressed
}

bool isReleased(bool oldState, bool newState)
{
	return (oldState == 1) && (newState == 0);
}

bool isHeld(bool oldState, bool newState)
{
	return oldState && newState;					// There's a case to be made for simply returning newState.
}

*/


enum MOUSE_BUTTONS
{
	LEFT = 0x0001,
	RIGHT = 0x0002,
};



class Mouse
{
private:
	KeyState m_mouseKeyStates[5];
	bool m_mouseOldState[5] = { false };
	bool m_mouseNewState[5] = { false };
	COORD m_oldMouseLocation;
	COORD m_newMouseLocation;

	HANDLE m_hConsoleIn = GetStdHandle(STD_INPUT_HANDLE);


public:
	Mouse(HANDLE &consoleIn) { m_hConsoleIn = consoleIn; }			

	KeyState getState(int i)
	{
		return m_mouseKeyStates[i];
	}

	void update()
	{

		// Handle Mouse Input - Check for window events
		// An INPUT_RECORD consists of :

	/*
		typedef struct _INPUT_RECORD {
	  WORD  EventType;
	  union {
		KEY_EVENT_RECORD          KeyEvent;
		MOUSE_EVENT_RECORD        MouseEvent;
		WINDOW_BUFFER_SIZE_RECORD WindowBufferSizeEvent;
		MENU_EVENT_RECORD         MenuEvent;
		FOCUS_EVENT_RECORD        FocusEvent;
	  } Event;
	} INPUT_RECORD;
	*/

	/*

	MOUSE_EVENT_RECORD  looks like

	typedef struct _MOUSE_EVENT_RECORD {
	COORD dwMousePosition;
	DWORD dwButtonState;
	DWORD dwControlKeyState;
	DWORD dwEventFlags;
	} MOUSE_EVENT_RECORD;



	*/
		INPUT_RECORD inBuf[32];		// Assuming we won't have more than 128 keyboard + mouse inputs between frames...  Seems safe.. (?)
		DWORD events = 0;
		GetNumberOfConsoleInputEvents(m_hConsoleIn, &events);
		if (events > 0)
			ReadConsoleInput(m_hConsoleIn, inBuf, events, &events);		// first events: Size of read buffer.  Second events: number of records read.


		for (DWORD i = 0; i < events; i++)			// Update mouse state.  i.e. m_newMouseLocation and m_mouseNewState
													// REFACTOR:::  make these names consistent.  I.E. both m_newMouseBlah or both m_mouseNewBlah
		{
			if (inBuf[i].EventType == MOUSE_EVENT)
			{
				m_newMouseLocation = inBuf[i].Event.MouseEvent.dwMousePosition;		// Update mouse location
				//m_mousePosX = inBuf[i].Event.MouseEvent.dwMousePosition.X;			Maybe we we want to work with x and y directly...
				//m_mousePosY = inBuf[i].Event.MouseEvent.dwMousePosition.Y;			In that case, change the class defn and uncomment



				for (int i = 0; i < 5; i++)											// Update which mouse buttons are pressed
					m_mouseNewState[i] = ( (inBuf[i].Event.MouseEvent.dwButtonState & (1 << i)) != 0 );		// Mouse buttons are flags all stored in dwButtonState
				
			}
		}



		for (int i = 0; i < 5; i++)
		{
			m_mouseKeyStates[i].bPressed = isPressed(m_mouseOldState[i], m_mouseNewState[i]);
			m_mouseKeyStates[i].bHeld = isHeld(m_mouseOldState[i], m_mouseNewState[i]);
			m_mouseKeyStates[i].bReleased = isReleased(m_mouseOldState[i], m_mouseNewState[i]);

			

			m_mouseOldState[i] = m_mouseNewState[i];
		}


	}


};










#endif
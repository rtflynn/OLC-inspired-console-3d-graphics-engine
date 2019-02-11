#ifndef KEYBOARD_H
#define KEYBOARD_H
#include "keystate.h"
#pragma comment(lib, "user32.lib")					// For GetAsyncKeyState




class Keyboard
{
private:
	KeyState m_keys[256];
	bool m_keyOldState[256] = { false };
	bool m_keyNewState[256] = { false };

public:
	Keyboard() {}

	void update()
	{
		for (int i = 0; i < 256; i++)
		{
			m_keyNewState[i] = GetAsyncKeyState(i);
			m_keys[i].bHeld = isHeld(m_keyOldState[i], m_keyNewState[i]);
			m_keys[i].bPressed = isPressed(m_keyOldState[i], m_keyNewState[i]);
			m_keys[i].bReleased = isReleased(m_keyOldState[i], m_keyNewState[i]);
			m_keyOldState[i] = m_keyNewState[i];
		}
	}

	KeyState getState(int i)
	{
		if (i < 0 || i > 255)
			return KeyState();
		return m_keys[i];
	}

};













#endif
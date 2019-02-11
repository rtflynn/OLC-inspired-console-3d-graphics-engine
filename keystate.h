#ifndef KEYSTATE_H
#define KEYSTATE_H

struct KeyState
{
	bool bPressed = false;
	bool bReleased = false;
	bool bHeld = false;
};


bool isPressed(bool oldState, bool newState);
bool isReleased(bool oldState, bool newState);
bool isHeld(bool oldState, bool newState);


#endif
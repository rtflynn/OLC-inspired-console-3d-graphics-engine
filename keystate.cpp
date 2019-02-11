#include "keystate.h"

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
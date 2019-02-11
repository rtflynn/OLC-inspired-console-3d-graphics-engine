#ifndef DRAWER_H
#define DRAWER_H

#include "Console.h"


// Eventually refactor with a Point2D class

class Drawer : public Console
{
public:

	void Clamp(int &x, int &y);

	void FillRectangle(int x1, int y1, int x2, int y2, short character = 0x2588, short color = 0x000F);

	void DrawString(int x, int y, std::wstring characters, short color = 0x000F);

	void DrawStringAlpha(int x, int y, std::wstring characters, short color = 0x000F);

	void DrawLine(const int x1, const int y1, const int x2, const int y2, const short character, const short color);

	void DrawHorizontalLine(int x1, int y1, int x2, int y2, short character, short color);

	void DrawTriangle(int x1, int y1, int x2, int y2, int x3, int y3, short character = 0x2588, short color = 0x000F);

	void FillTriangle(int x1, int y1, int x2, int y2, int x3, int y3, short character = 0x2588, short color = 0x000F);


};

#endif
#include "Drawer.h"
#include <math.h>

void Drawer::Clamp(int &x, int &y)
{
	if (x < 0) x = 0;
	if (x >= m_nScreenWidth) x = m_nScreenWidth;
	if (y < 0) y = 0;
	if (y >= m_nScreenHeight) y = m_nScreenHeight;
}

void Drawer::FillRectangle(int x1, int y1, int x2, int y2, short character, short color)
{
	Clamp(x1, y1);
	Clamp(x2, y2);
	for (int x = x1; x < x2; x++)
		for (int y = y1; y < y2; y++)
			Draw(x, y, character, color);
}

void Drawer::DrawString(int x, int y, std::wstring characters, short color)
{
	for (size_t i = 0; i < characters.size(); i++)
	{
		m_screenBuffer[y * m_nScreenWidth + x + i].Char.UnicodeChar = characters[i];
		m_screenBuffer[y * m_nScreenWidth + x + i].Attributes = color;
	}
}

void Drawer::DrawStringAlpha(int x, int y, std::wstring characters, short color)
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

void Drawer::DrawLine(const int x1, const int y1, const int x2, const int y2, const short character = 0x2588, const short color = 0x000F)	
{
	float xCurrent = (float)x1;
	float yCurrent = (float)y1;
	float xDist = (float)(x2 - x1);
	float yDist = (float)(y2 - y1);
	float totalDistance = sqrt(xDist*xDist + yDist * yDist);
	int approxTotalDistance = round(totalDistance);
	//int samplePointFrequency = 1;
	//float samplePointDistance = 1 / samplePointFrequency;
	float samplePointDistance;
	int numberOfSamplePoints = max(xDist, yDist);
	if (xDist < yDist)
		samplePointDistance = totalDistance / yDist;
	else samplePointDistance = totalDistance / xDist;
	
	//int numberOfSamplePoints = samplePointFrequency * approxTotalDistance;
	
	float samplePointDeltaX = xDist / numberOfSamplePoints;
	float samplePointDeltaY = yDist / numberOfSamplePoints;

	for (int i = 0; i < numberOfSamplePoints; i++)
	{
		Draw(round(xCurrent), round(yCurrent), character, color);
		xCurrent += samplePointDeltaX;
		yCurrent += samplePointDeltaY;
	}
}

////////// By the end of this project, either const-qualify everything for which it makes sense, or revert DrawLine to not be const-qualified.
////////// At the moment still deciding if the drop-off in readability from using const is worth the benefit of safety, plus broadcasting
////////// our intentions.

void Drawer::DrawTriangle(int x1, int y1, int x2, int y2, int x3, int y3, short character, short color)		// Maybe refactor with Points instead of Ordered Pairs
{
	DrawLine(x1, y1, x2, y2, character, color);
	DrawLine(x2, y2, x3, y3, character, color);
	DrawLine(x3, y3, x1, y1, character, color);
}

void Drawer::DrawHorizontalLine(int x1, int y1, int x2, int y2, short character, short color)
{
	int xMin = min(x1, x2);
	int xMax = max(x1, x2);
	for (int i = xMin ; i <= xMax; i++)
	{
		Draw(i, y1);
	}
}

const int tempWidth = 256;
const int tempHeight = 200;								// Eventually need to refactor so this comes from the Console height...
int leftRightArray[tempHeight][2];						// store left and right boundaries of cross sections of triangle here

void scanLine(int x1, int y1, int x2, int y2)
{	
	int dx, dy, currentx, currenty, xstepPrimary, ystepPrimary, xstepSecondary, ystepSecondary, absdy, absdx, numSteps, longside, shortside;
	
	dx = x2 - x1;		absdx = abs(dx);
	dy = y2 - y1;		absdy = abs(dy);

	if (dx > 0) xstepPrimary = 1;
	else if (dx < 0) xstepPrimary = -1;
	else xstepPrimary = 0;

	if (dy > 0) ystepPrimary = 1;
	else if (dy < 0) ystepPrimary = -1;
	else ystepPrimary = 0;

	if (absdy <= absdx)
	{
		longside = absdx;
		shortside = absdy;
		xstepSecondary = xstepPrimary;
		ystepSecondary = 0;
	}

	else
	{
		longside = absdy;
		shortside = absdx;
		xstepSecondary = 0;
		ystepSecondary = ystepPrimary;
	}

	currentx = x1;
	currenty = y1;
	numSteps = longside + 1;
	int primaryStepAccumulatedError = shortside / 2;


	while (numSteps--)
	{
		if ((currenty >= 0) && (currenty < tempHeight))
		{
			if (currentx < leftRightArray[currenty][0])		leftRightArray[currenty][0] = currentx;
			if (currentx > leftRightArray[currenty][1])		leftRightArray[currenty][1] = currentx;
		}

		primaryStepAccumulatedError += shortside;
		if (primaryStepAccumulatedError < longside)
		{
			currentx += xstepSecondary;
			currenty += ystepSecondary;
		}

		else
		{
			currentx += xstepPrimary;
			currenty += ystepPrimary;
			primaryStepAccumulatedError -= longside;
		}
	}
}

void Drawer::FillTriangle(int x1, int y1, int x2, int y2, int x3, int y3, short character, short color) 
{
	int y;
	for (y = 0; y < tempHeight; y++)
	{
		leftRightArray[y][0] = INT_MAX;
		leftRightArray[y][1] = INT_MIN;
	}

	scanLine(x1, y1, x2, y2);
	scanLine(x1, y1, x3, y3);
	scanLine(x2, y2, x3, y3);


	for (y = 0; y < tempHeight; y++)
	{
		if (leftRightArray[y][0] <= leftRightArray[y][1])
		{
			for (int x = leftRightArray[y][0]; x <= leftRightArray[y][1]; x++)
				Draw(x, y, character, color);
		}
	}
}



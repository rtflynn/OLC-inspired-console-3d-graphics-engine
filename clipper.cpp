#include "clipper.h"
#include <list>


vec3d intersectionOfLineAndPlane(vec3d &pointOnPlane, vec3d &normalVectorToPlane, vec3d &firstEndpoint, vec3d &lastEndpoint)
{
	vec3d lineDirection = lastEndpoint - firstEndpoint;
	float planeEqnConstant = dotProduct(normalVectorToPlane, pointOnPlane);
	float intersectionTime = (planeEqnConstant - dotProduct(normalVectorToPlane, firstEndpoint)) / (dotProduct(normalVectorToPlane, lineDirection));

	vec3d intersectionPoint = intersectionTime * lineDirection + firstEndpoint;
	return intersectionPoint;
}


bool onCorrectSideOfPlane(vec3d &pointOnPlane, vec3d &normalVectorToPlane, vec3d &somePoint)
{
	return (dotProduct(normalVectorToPlane, somePoint - pointOnPlane) >= 0);
}


int numVerticesOnCorrectSide(vec3d &pointOnPlane, vec3d &normalVectorToPlane, triangle &tri)
{
	return (onCorrectSideOfPlane(pointOnPlane, normalVectorToPlane, tri.p[0]) +
		onCorrectSideOfPlane(pointOnPlane, normalVectorToPlane, tri.p[1]) +
		onCorrectSideOfPlane(pointOnPlane, normalVectorToPlane, tri.p[2])
		);
}


void rotateTriangle(triangle &tri)
{
	vec3d temp = tri.p[0];
	tri.p[0] = tri.p[1];
	tri.p[1] = tri.p[2];
	tri.p[2] = temp;
}


void clipByPlane(vec3d &pointOnPlane, vec3d &normalVectorToPlane, std::list<triangle> &tris, bool clipping_debug)    
{
	for (std::list<triangle>::iterator i = tris.begin(); i != tris.end(); )
	{
		triangle &tri = *i;
		int numVerts = numVerticesOnCorrectSide(pointOnPlane, normalVectorToPlane, tri);
		if (numVerts == 3)
		{
			i++;
		}
		
		else if (numVerts == 2)
		{
			while (onCorrectSideOfPlane(pointOnPlane, normalVectorToPlane, tri.p[0]))		// Make sure that vertex 0 of our triangle is the one on the wrong side
				rotateTriangle(tri);
			vec3d intersection1 = intersectionOfLineAndPlane(pointOnPlane, normalVectorToPlane, tri.p[0], tri.p[1]);
			vec3d intersection2 = intersectionOfLineAndPlane(pointOnPlane, normalVectorToPlane, tri.p[0], tri.p[2]);
			triangle tri1{ tri.p[1], tri.p[2], intersection1 };
			triangle tri2{ tri.p[2], intersection2, intersection1 };
			tri1.color = tri.color;
			tri1.symbol = tri.symbol;
			tri2.color = tri.color;
			tri2.symbol = tri.symbol;

			if (clipping_debug){
			tri1.color = FG_RED;							
			tri1.symbol = PIXEL_SOLID;
			tri2.color = FG_YELLOW;
			tri2.symbol = PIXEL_SOLID; }
			
			i = tris.erase(i);						
			tris.insert(i, tri1);
			tris.insert(i, tri2);
		}

		else if (numVerts == 1)
		{
			triangle clippedTriangle;
			while (!onCorrectSideOfPlane(pointOnPlane, normalVectorToPlane, tri.p[0]))
				rotateTriangle(tri);
			vec3d intersection1 = intersectionOfLineAndPlane(pointOnPlane, normalVectorToPlane, tri.p[0], tri.p[1]);
			vec3d intersection2 = intersectionOfLineAndPlane(pointOnPlane, normalVectorToPlane, tri.p[0], tri.p[2]);
			triangle tri1{ tri.p[0], intersection1, intersection2 };


			tri1.color = tri.color;
			tri1.symbol = tri.symbol;

			if (clipping_debug) {
			tri1.color = FG_GREEN;						
			tri1.symbol = PIXEL_SOLID;}

			i = tris.erase(i);				
			tris.insert(i, tri1);
		}

		else if (numVerts == 0)
		{
			i = tris.erase(i);					
		}

		else {
			std::cout << "That's not supposed to happen..." << std::endl; 
			assert(1 == 0);
		}
	}
}
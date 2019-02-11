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


void clipByPlane(vec3d &pointOnPlane, vec3d &normalVectorToPlane, std::list<triangle> &tris)    
{
	for (std::list<triangle>::iterator i = tris.begin(); i != tris.end(); )
	{
		triangle &tri = *i;
		int numVerts = numVerticesOnCorrectSide(pointOnPlane, normalVectorToPlane, tri);
		if (numVerts == 3)
		{
			i++;
			//return;
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

//			tri1.color = FG_RED;							// For debug
//			tri1.symbol = PIXEL_SOLID;
//			tri2.color = FG_YELLOW;
//			tri2.symbol = PIXEL_SOLID;
			tri2.color = tri.color;
			tri2.symbol = tri.symbol;

			triangle temptri1{ tri.p[2], tri.p[1], intersection1 };				// What's this for? delete it.
			triangle temptri2{ tri.p[2], intersection1, intersection2 };
			
			tris.insert(i, tri1);
			tris.insert(i, tri2);
	//		i = tris.erase(i);
			i++;

		}

		else if (numVerts == 1)
		{
			triangle clippedTriangle;
			while (!onCorrectSideOfPlane(pointOnPlane, normalVectorToPlane, tri.p[0]))
				rotateTriangle(tri);
			vec3d intersection1 = intersectionOfLineAndPlane(pointOnPlane, normalVectorToPlane, tri.p[0], tri.p[1]);
			vec3d intersection2 = intersectionOfLineAndPlane(pointOnPlane, normalVectorToPlane, tri.p[0], tri.p[2]);
			triangle tri1{ tri.p[0], intersection1, intersection2 };

//			tri1.color = FG_GREEN;						// Good for debug.
//			tri1.symbol = PIXEL_SOLID;

			tri1.color = tri.color;
			tri1.symbol = tri.symbol;

			tris.insert(i, tri1);
	//		i = tris.erase(i);
			i++;
		}

		else if (numVerts == 0)
		{
		//	i = tris.erase(i);
			i++;
		}

		else {
			std::cout << "That's not supposed to happen..." << std::endl; 
			assert(1 == 0);
		}

	}



}











/*

std::vector<triangle> clipByZCoordinate(std::vector<triangle> const &triangles)
{
	std::vector<triangle> clippedTriangles;
	vec3d zPlanePoint{ 0.0f, 0.0f, .05f };
	vec3d zPlaneNormal{ 0.0f, 0.0f, 1.0f };
	for (triangle tri : triangles)
	{
		clipByPlane(zPlanePoint, zPlaneNormal, tri, clippedTriangles);
	}

	return clippedTriangles;
}
*/


//// Holy crap --- OK I finally understand why this was bugging out soooo badly.
//// The bug was:  if zPlanePoint = vec3d{0.0f, 0.0f, z} with some float z, 
//// then for z < 1 nothing was clipped and for z > 1 everything was clipped.
//// Well, this is because we're clipping AFTER projecting, so the z coord
//// of every triangle is 1.  DERP DERP DERP
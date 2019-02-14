#ifndef CLIPPER_H
#define CLIPPER_H

#include "vec3d.h"
#include "triangle.h"
#include <vector>
#include <list>



vec3d intersectionOfLineAndPlane(vec3d &pointOnPlane, vec3d &normalVectorToPlane, vec3d &firstEndpoint, vec3d &lastEndpoint);

bool onCorrectSideOfPlane(vec3d &pointOnPlane, vec3d &normalVectorToPlane, vec3d &somePoint);

int numVerticesOnCorrectSide(vec3d &pointOnPlane, vec3d &normalVectorToPlane, triangle &tri);

void rotateTriangle(triangle &tri);

void clipByPlane(vec3d &pointOnPlane, vec3d &normalVectorToPlane, std::list<triangle> &clippedTriangles, bool clipping_debug = true);

std::vector<triangle> clipByZCoordinate(std::vector<triangle> const &triangles);





#endif
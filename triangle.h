#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "vec3d.h"
#include "mat4x4.h"
#include "pixel_enums.h"

struct triangle
{
	vec3d p[3];
	wchar_t symbol;
	short color = FG_DARK_YELLOW;
};




bool operator==(triangle const &tri1, triangle const &tri2);

triangle operator+(triangle const &tri, vec3d const &shift);

triangle operator-(triangle const &tri, vec3d const &shift);

void componentwiseMultiply(const vec3d &i, triangle &tri);

vec3d midpointOfTriangle(const triangle tri);

vec3d normalVectorToTriangle(triangle const &tri);

triangle operator*(mat4x4 const &m, triangle const &tri);

#endif
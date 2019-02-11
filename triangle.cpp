#include "triangle.h"

struct triangle;

bool operator==(triangle const &tri1, triangle const &tri2)
{
	return (tri1.p == tri2.p) and (tri1.color == tri2.color) and (tri1.symbol == tri2.symbol);
}


triangle operator+(triangle const &tri, vec3d const &shift)
{
	triangle result;
	result.p[0] = tri.p[0] + shift;
	result.p[1] = tri.p[1] + shift;
	result.p[2] = tri.p[2] + shift;
	result.symbol = tri.symbol;
	result.color = tri.color;
	return result;
}


triangle operator-(triangle const &tri, vec3d const &shift)
{
	triangle result;
	result.p[0] = tri.p[0] - shift;
	result.p[1] = tri.p[1] - shift;
	result.p[2] = tri.p[2] - shift;
	result.symbol = tri.symbol;
	result.color = tri.color;
	return result;
}


void componentwiseMultiply(const vec3d &i, triangle &tri)
{
	componentwiseMultiply(i, tri.p[0]);
	componentwiseMultiply(i, tri.p[1]);
	componentwiseMultiply(i, tri.p[2]);
}


vec3d midpointOfTriangle(const triangle tri)
{
	return (tri.p[0] + tri.p[1] + tri.p[2]) / 3.0f;
}


vec3d normalVectorToTriangle(triangle const &tri) {
	vec3d P = tri.p[1] - tri.p[0];
	vec3d Q = tri.p[2] - tri.p[1];
	return crossProduct(P, Q);
}


triangle operator*(mat4x4 const &m, triangle const &tri)
{
	triangle result;
	result.p[0] = m * tri.p[0];
	result.p[1] = m * tri.p[1];
	result.p[2] = m * tri.p[2];
	result.symbol = tri.symbol;
	result.color = tri.color;
	return result;
}
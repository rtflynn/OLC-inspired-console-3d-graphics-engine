#ifndef VEC3D_H
#define VEC3D_H

#include <cassert>
#include <iostream>

struct vec3d
{
	float x, y, z, proj=1.0f;
};

vec3d operator+(const vec3d &p, const vec3d &q);

vec3d operator-(const vec3d &p, const vec3d &q);

vec3d operator*(const float &someScalar, const vec3d &q);

vec3d operator/(const vec3d &q, const float &someScalar);

void componentwiseMultiply(const vec3d &i, vec3d &o);

float dotProduct(const vec3d &p, const vec3d &q);

vec3d crossProduct(vec3d const P, vec3d const Q);

void normalize(vec3d &v);


#endif
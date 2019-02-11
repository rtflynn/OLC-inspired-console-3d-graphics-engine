#ifndef MAT4X4_H
#define MAT4X4_H
#include "vec3d.h"
#include <iostream>


struct mat4x4
{
	float m[4][4] = { 0 };
};

vec3d operator*(mat4x4 const &m, vec3d const &i);

mat4x4 operator*(mat4x4 const &m1, mat4x4 const &m2);

mat4x4 makeProjectionMatrix(float fAspectRatio, float fNear = 0.1f, float fFar = 1000.0f, float fFov = 90.0f);

mat4x4 makeRotationMatrixZ(float fTheta);

mat4x4 makeRotationMatrixX(float fTheta);

mat4x4 makeRotationMatrixY(float fTheta);

mat4x4 matrixPointAt(vec3d &pos, vec3d &target, vec3d &up); 

mat4x4 quickInverse(mat4x4 const &m);		

#endif
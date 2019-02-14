#include "vec3d.h"

vec3d operator+(const vec3d &p, const vec3d &q)
{
	vec3d sum;
	assert(p.proj == 1 && q.proj == 1);
	sum.x = p.x + q.x;
	sum.y = p.y + q.y;
	sum.z = p.z + q.z;
	return sum;
}

vec3d operator-(const vec3d &p, const vec3d &q)
{
	vec3d difference;
	assert(p.proj == 1 && q.proj == 1);
	difference.x = p.x - q.x;
	difference.y = p.y - q.y;
	difference.z = p.z - q.z;
	return difference;
}

vec3d operator*(const float &someScalar, const vec3d &q)
{
	vec3d result;
	result.x = someScalar * q.x;
	result.y = someScalar * q.y;
	result.z = someScalar * q.z;
	return result;
}


vec3d operator/(const vec3d &q, const float &someScalar)
{
	if (someScalar == 0)
	{
		std::cout << "WOAH THERE NELLY" << std::endl;
	}
	vec3d result;
	result.x = q.x / someScalar;
	result.y = q.y / someScalar;
	result.z = q.z / someScalar;
	return result;
}


void componentwiseMultiply(const vec3d &i, vec3d &o)
{
	o.x *= i.x;
	o.y *= i.y;
	o.z *= i.z;
	o.proj *= i.proj;
}


float dotProduct(const vec3d &p, const vec3d &q)
{
	return p.x * q.x + p.y * q.y + p.z * q.z;
}



vec3d crossProduct(vec3d const P, vec3d const Q)
{
	vec3d result;
	result.x = P.y * Q.z - P.z * Q.y;
	result.y = P.z * Q.x - P.x * Q.z;
	result.z = P.x * Q.y - P.y * Q.x;

	return result;
}

void normalize(vec3d &v)
{
	float magnitude = sqrtf(dotProduct(v, v));
	if (magnitude != 0)
	{
		v.x = v.x / magnitude;
		v.y = v.y / magnitude;
		v.z = v.z / magnitude;
	}
}

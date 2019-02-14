#ifndef MESH_H
#define MESH_H

#include <fstream>
#include <strstream>
#include "triangle.h"
#include <vector>
#include "vec3d.h"


struct mesh
{
public:
	std::vector<triangle> tris;

	mesh operator+(vec3d translationMatrix);

	bool loadFromObjectFile(std::string fileName); 	
};

#endif
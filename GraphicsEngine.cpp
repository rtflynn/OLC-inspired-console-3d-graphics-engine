#include "Console.h"
#include <assert.h>
#include <fstream>			// fstream and strstream to read and parse objects from .obj files.
#include <strstream>		// Move this all once we refactor into separate headers.
#include <algorithm>		// For sorting triangles into painters order
#include "vec3d.h"
#include "mat4x4.h"
#include "triangle.h"
#include "mesh.h"
#include "olcEngine3D.h"


int main()
{
	olcEngine3D demo;
	demo.SetConsole(256, 200, 4, 4);			// (256, 200, 4, 4)
	demo.Start();

	return 0;
}
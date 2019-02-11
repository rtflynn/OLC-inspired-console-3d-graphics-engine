#include "mesh.h"





	mesh mesh::operator+(vec3d translationMatrix)
	{
		mesh newMesh;
		for (auto tri : tris) {
			triangle tempTri = tri + translationMatrix;
			newMesh.tris.push_back(tempTri);
		}

		return newMesh;
	}

	bool mesh::loadFromObjectFile(std::string fileName)
	{
		std::ifstream objFile(fileName);						// The file is loaded into an fstream
		if (!objFile.is_open())
			return false;

		std::vector<vec3d> verts;
		while (!objFile.eof())									// loop until we reach end of file
		{
			char line[128];										// Assuming no line of our file is longer than 128 chars.  Fix later!
			objFile.getline(line, 128);
			std::strstream s;									// Turn the line into a stringstream --- easier to work with than a char buffer
			s << line;

			char junk;											// The first char of each line in an .obj file is throwaway after being parsed

			if (line[0] == 'v')									// Lines in an object file look like  v 0.72000 0.12000 -1.4000  for vertices
																// and  f 21 52 12   for triangles (faces).  
			{
				vec3d v;
				s >> junk >> v.x >> v.y >> v.z;
				verts.push_back(v);
			}

			else if (line[0] == 'f')
			{
				int F[3];
				s >> junk >> F[0] >> F[1] >> F[2];
				tris.push_back({ verts[F[0] - 1], verts[F[1] - 1], verts[F[2] - 1] });
				// Unfortunately, .obj files store vertices starting at 1, thus the need to subtract 1.  A potentially much more performant
				// solution would be to throw a throwaway vertex to verts before all the ones from the .obj file.
				// However this will only affect loading times, not running time, because each object is loaded just once then stored in memory.
			}
		}

		objFile.close();
		return true;
	}


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
	std::ifstream objFile(fileName);						
	if (!objFile.is_open())
		return false;

	std::vector<vec3d> verts;
	verts.push_back(vec3d{ 0.0f,0.0f,0.0f });				// .obj files store vertices starting at 1, so add in a 'filler' vertex for later
		
	while (!objFile.eof())									
	{
		char line[128];										
		objFile.getline(line, 128);
		std::strstream s;								
		s << line;

		char junk;											

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
			tris.push_back({ verts[F[0]], verts[F[1]], verts[F[2]] });  // Without the 'filler' vertex, these would be F[i] - 1
		}
	}

	objFile.close();
	return true;
}


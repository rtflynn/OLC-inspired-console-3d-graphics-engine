#ifndef OLCENGINE3D_H
#define OLCENGINE3D_H
#include "mesh.h"
#include "mat4x4.h"
#include "Console.h"
#include "vec3d.h"
#include "triangle.h"
#include <algorithm>
#include "Drawer.h"
#include "clipper.h"
#include "basicShapes.h"


class olcEngine3D : public Drawer
{

public:
	olcEngine3D() { m_sAppName = L"3D Engine"; }

private:
	std::vector<mesh> myObjects;
	mat4x4 matProj, rotationTranslationMatrix;
	float fThetaX = 0.0f, fThetaZ = 0.0f, fYaw = 0.0f;
	vec3d translation{ 0.0f, 0.0f, 8.0f };

	mat4x4 matNewCoordsToOriginal{ 0.0f };
	vec3d cameraLocation{ 0.0f };

	CHAR_INFO GetColour(float luminosity);		
	void applyKeyboardInputsToRotationsAndTranslations(float fElapsedTime);
	mat4x4 makeRotTranMat(float fElapsedTime);

public:
	bool OnUserCreate() override;

	void clearScreen();	
	
	void drawAllTriangles(std::vector<triangle> &trianglesInPaintersOrder, bool debug);

	void sortByZComponent(std::vector<triangle> &trianglesInPaintersOrder);

	void convertToCameraCoordsAndZClip(mesh &myMesh, std::list<triangle> &trianglesBeingProcessed);

	void cullAccordingToOrientation(std::list<triangle> &trianglesBeingProcessed);

	void applyLighting(std::list<triangle> &triangles, vec3d lightDirection);

	void projectTrianglesToScreen(std::list<triangle> &triangles);

	void updateViewMatrices(float fElapsedTime);

	std::list<triangle> collectAllTrianglesInOnePlace();

	void translateAndRotateToCameraSpace(std::list<triangle> &ourTriangles);

	void cullAndApplyLighting(std::list<triangle> &ourTriangles);

	void screenBorderClip(std::list<triangle> &tris);

	std::vector<triangle> convertListToVector(std::list<triangle> &tris);

	void sortTrianglesByZCoord(std::vector<triangle> &tris);

	bool OnUserUpdate(float fElapsedTime) override;

};

#endif
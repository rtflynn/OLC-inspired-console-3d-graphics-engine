#include "olcEngine3D.h"


	CHAR_INFO olcEngine3D::GetColour(float luminosity)		// copy/paste from OneLoneCoder
	{
		short bg_col, fg_col;
		wchar_t sym;
		int pixel_bw = (int)(13.0f*luminosity);
		switch (pixel_bw)
		{
		case 0: bg_col = BG_BLACK; fg_col = FG_BLACK; sym = PIXEL_SOLID; break;

		case 1: bg_col = BG_BLACK; fg_col = FG_DARK_GREY; sym = PIXEL_QUARTER; break;
		case 2: bg_col = BG_BLACK; fg_col = FG_DARK_GREY; sym = PIXEL_HALF; break;
		case 3: bg_col = BG_BLACK; fg_col = FG_DARK_GREY; sym = PIXEL_THREEQUARTERS; break;
		case 4: bg_col = BG_BLACK; fg_col = FG_DARK_GREY; sym = PIXEL_SOLID; break;

		case 5: bg_col = BG_DARK_GREY; fg_col = FG_GREY; sym = PIXEL_QUARTER; break;
		case 6: bg_col = BG_DARK_GREY; fg_col = FG_GREY; sym = PIXEL_HALF; break;
		case 7: bg_col = BG_DARK_GREY; fg_col = FG_GREY; sym = PIXEL_THREEQUARTERS; break;
		case 8: bg_col = BG_DARK_GREY; fg_col = FG_GREY; sym = PIXEL_SOLID; break;

		case 9:  bg_col = BG_GREY; fg_col = FG_WHITE; sym = PIXEL_QUARTER; break;
		case 10: bg_col = BG_GREY; fg_col = FG_WHITE; sym = PIXEL_HALF; break;
		case 11: bg_col = BG_GREY; fg_col = FG_WHITE; sym = PIXEL_THREEQUARTERS; break;
		case 12: bg_col = BG_GREY; fg_col = FG_WHITE; sym = PIXEL_SOLID; break;
		default:
			bg_col = BG_BLACK; fg_col = FG_BLACK; sym = PIXEL_SOLID;
		}

		CHAR_INFO c;
		c.Attributes = bg_col | fg_col;
		c.Char.UnicodeChar = sym;
		return c;
	}

	void olcEngine3D::applyKeyboardInputsToRotationsAndTranslations(float fElapsedTime)
	{
		if (m_keyboard.getState(0x25).bHeld)
		{
			cameraLocation.x -= 3.0*fElapsedTime;
			//			fThetaZ += 1.0f * fElapsedTime;
		}
		if (m_keyboard.getState(0x27).bHeld)
		{
			cameraLocation.x += 3.0 * fElapsedTime;
			//			fThetaZ -= 1.0f * fElapsedTime;
		}
		if (m_keyboard.getState(0x26).bHeld) {
			cameraLocation.y += 3.0 * fElapsedTime;
			//			fThetaX += 1.0f * fElapsedTime;
		}
		if (m_keyboard.getState(0x28).bHeld) {
			cameraLocation.y -= 3.0 * fElapsedTime;

			//			fThetaX -= 1.0f * fElapsedTime;
		}

		// WSAD to move the camera forward/backward/left/right
		if (m_keyboard.getState(0x57).bHeld)
		{
			//translation.z -= 3.0*fElapsedTime;
			cameraLocation.z += 1.0 * fElapsedTime;			// These are equivalent.  Do we want to think about translating the objects or the camera?
		}
		if (m_keyboard.getState(0x53).bHeld)
		{
			cameraLocation.z -= 1.0 * fElapsedTime;
			//translation.z += 3.0*fElapsedTime;
		}
		if (m_keyboard.getState(0x41).bHeld)
		{
			translation.x += 3.0*fElapsedTime;
			//fYaw += 1.0*fElapsedTime;
		}
		if (m_keyboard.getState(0x44).bHeld)
		{
			//fYaw -= 1.0*fElapsedTime;
			translation.x -= 3.0*fElapsedTime;
		}
	}

	mat4x4 olcEngine3D::makeRotTranMat(float fElapsedTime)
	{
		mat4x4 matRotZ, matRotX, matRotY;
		applyKeyboardInputsToRotationsAndTranslations(fElapsedTime);

		matRotZ = makeRotationMatrixZ(fThetaZ);
		matRotX = makeRotationMatrixX(fThetaX);
		matRotY = makeRotationMatrixY(fYaw);

		mat4x4 rotationTranslationMatrix = matRotZ * matRotX;

		rotationTranslationMatrix.m[3][0] += translation.x;
		rotationTranslationMatrix.m[3][1] += translation.y;
		rotationTranslationMatrix.m[3][2] += translation.z;

		return rotationTranslationMatrix;
	}

	bool olcEngine3D::OnUserCreate() 
	{

		/*
		mesh someMesh;
//		someMesh = unitCube();// +vec3d({ 0.0f, 0.0f, 0.0f });
		someMesh.loadFromObjectFile("teapot.obj");			// Interesting history on the Utah teapot
		myObjects.push_back(someMesh);						// At the moment this is causing problems with division by 0, i.e. the projection matmul
															// is giving a w value of 0.  Probs cause this goes through origin etc.

		mesh someOtherMesh;
		someOtherMesh.loadFromObjectFile("teapot.obj");
		myObjects.push_back(someOtherMesh + vec3d({ 7.0f, 0.0f, 10.0f }));		

		mesh someOtherOtherMesh;
		someOtherOtherMesh.loadFromObjectFile("teapot.obj");
		myObjects.push_back(someOtherOtherMesh + vec3d({ -7.0f, 0.0f, 5.0f }));
		*/



		mesh someEasierMesh;
		someEasierMesh.loadFromObjectFile("VideoShip.obj");
		myObjects.push_back(someEasierMesh);




		float fAspectRatio = (float)ScreenHeight() / (float)ScreenWidth();
		matProj = makeProjectionMatrix(fAspectRatio);
		return true;
	}
	
	void olcEngine3D::clearScreen()
	{
		FillRectangle(0, 0, ScreenWidth(), ScreenHeight(), PIXEL_SOLID, FG_BLACK);
	}

	void olcEngine3D::drawAllTriangles(std::vector<triangle> &trianglesInPaintersOrder, bool debug = FALSE)
	{
		for (auto const &triProjected : trianglesInPaintersOrder)
		{
			FillTriangle(triProjected.p[0].x, triProjected.p[0].y,
				triProjected.p[1].x, triProjected.p[1].y,
				triProjected.p[2].x, triProjected.p[2].y,
				triProjected.symbol, triProjected.color);

			if (debug) {
				DrawTriangle(triProjected.p[0].x, triProjected.p[0].y,
					triProjected.p[1].x, triProjected.p[1].y,
					triProjected.p[2].x, triProjected.p[2].y,
					triProjected.symbol, FG_CYAN);
			}

		}
	}


	void olcEngine3D::sortByZComponent(std::vector<triangle> &trianglesInPaintersOrder)
	{

		std::sort(trianglesInPaintersOrder.begin(), trianglesInPaintersOrder.end(),
			[](triangle const &t1, triangle const &t2) {
			float z1 = t1.p[0].z + t1.p[1].z + t1.p[2].z;
			float z2 = t2.p[0].z + t2.p[1].z + t2.p[2].z;
			return z1 > z2; });
	}


	void olcEngine3D::convertToCameraCoordsAndZClip(mesh &myMesh, std::list<triangle> &trianglesBeingProcessed)
	{
		vec3d zPlanePoint{ 0.0f, 0.0f, .1f };
		vec3d zPlaneNormal{ 0.0f, 0.0f, 1.0f };
		for (triangle tri : myMesh.tris)
		{
			triangle triTranslated;
			triTranslated = rotationTranslationMatrix * tri;
			triTranslated = matNewCoordsToOriginal * triTranslated;
			std::vector<triangle> clippedTriangles;
//			clipByPlane(zPlanePoint, zPlaneNormal, triTranslated, clippedTriangles);

			trianglesBeingProcessed.insert(trianglesBeingProcessed.end(), clippedTriangles.begin(), clippedTriangles.end());
		}
	}



	void olcEngine3D::cullAccordingToOrientation(std::list<triangle> &trianglesBeingProcessed)
	{
		std::list<triangle>::iterator i = trianglesBeingProcessed.begin();
		while(i != trianglesBeingProcessed.end())
		{
			vec3d normal = normalVectorToTriangle(*i);
			if (dotProduct(normal, (*i).p[0] - cameraLocation) >= 0)
			{
				i = trianglesBeingProcessed.erase(i);
			}
		}
	}


	void olcEngine3D::applyLighting(std::list<triangle> &triangles, vec3d lightDirection)
	{
		for (triangle &tri : triangles)
		{
			vec3d normal = normalVectorToTriangle(tri);
			float length = sqrtf(dotProduct(normal, normal));
			if (length > 0)
				normal = normal / length;

			float lightDirectionLength = sqrtf(dotProduct(lightDirection, lightDirection));
			if (lightDirectionLength > 0)
				lightDirection = lightDirection / lightDirectionLength;

			float naiveLuminosity = dotProduct(normal, lightDirection);
			CHAR_INFO triangleColor = GetColour(naiveLuminosity);

			tri.symbol = triangleColor.Char.UnicodeChar;
			tri.color = triangleColor.Attributes;
		}
	}


	void olcEngine3D::projectTrianglesToScreen(std::list<triangle> &triangles)
	{
		for (triangle &tri : triangles)
		{			
			tri = matProj * (tri - cameraLocation);

			vec3d shiftIntoView{ 1.0f, 1.0f, 0.0f };
			tri = tri + shiftIntoView;

			vec3d scalingVector{ 0.5f * (float)(ScreenWidth()), 0.5f*(float)(ScreenHeight()), 1.0f };
			componentwiseMultiply(scalingVector, tri);
		}
	}

	
	
	// This is our "Cheap Clipping Procedure".  Either use this or expensive clipping, depending on the value of some boolean.
	// This is very significantly faster than the true clipping procedure.
	void olcEngine3D::clipByScreenDimensions(std::vector<triangle> &triangles2D, std::vector<triangle> &trianglesToRasterize)
	{


		vec3d leftWallPoint{ 0.0f, 0.0f, 0.0f };
		vec3d leftWallNormal{ 1.0f, 0.0f, 0.0f };
		vec3d topWallPoint{ 0.0f, 0.0f, 0.0f };
		vec3d topWallNormal{ 0.0f, 1.0f, 0.0f };
		vec3d rightWallPoint{ ScreenWidth() - 1, 0.0f, 0.0f };
		vec3d rightWallNormal{ -1.0f, 0.0f, 0.0f };
		vec3d bottomWallPoint{ 0.0f, ScreenHeight() - 1, 0.0f };
		vec3d bottomWallNormal{ 0.0f, -1.0f, 0.0f };
		std::list<triangle> leftClippedTriangles;


		for (triangle tri : triangles2D)
		{

			float minX = min(tri.p[0].x, tri.p[1].x, tri.p[2].x);
			float maxX = max(tri.p[0].x, tri.p[1].x, tri.p[2].x);
			float minY = min(tri.p[0].y, tri.p[1].y, tri.p[2].y);
			float maxY = max(tri.p[0].y, tri.p[1].y, tri.p[2].y);

			if ((0 <= minX) and (maxX <= ScreenWidth() - 1) and (0 <= minY) and (maxY <= ScreenHeight() - 1))
				trianglesToRasterize.push_back(tri);
			//else
			//{
			//	std::vector<triangle> clippedTriangles;
			//	clipByPlane(leftWallPoint, leftWallNormal, tri, clippedTriangles);
			//	leftClippedTriangles.insert(leftClippedTriangles.end(), clippedTriangles.begin(), clippedTriangles.end());
			//}
		}

		/*
		for (triangle tri : leftClippedTriangles)
		{
		std::vector<triangle> clippedTriangles;
		clipByPlane(topWallPoint, topWallNormal, tri, clippedTriangles);
		leftAndTopClippedTriangles.insert(leftAndTopClippedTriangles.end(), clippedTriangles.begin(), clippedTriangles.end());
		}

		vec3d rightWallPoint{ ScreenWidth() - 1, 0.0f, 0.0f };
		vec3d rightWallNormal{ -1.0f, 0.0f, 0.0f };
		std::list<triangle> leftAndTopAndRightClippedTriangles;
		for (triangle tri : leftAndTopClippedTriangles)
		{
		std::vector<triangle> clippedTriangles;
		clipByPlane(rightWallPoint, rightWallNormal, tri, clippedTriangles);
		leftAndTopAndRightClippedTriangles.insert(leftAndTopAndRightClippedTriangles.end(), clippedTriangles.begin(), clippedTriangles.end());
		}

		vec3d bottomWallPoint{ 0.0f, ScreenHeight() - 1, 0.0f };
		vec3d bottomWallNormal{ 0.0f, -1.0f, 0.0f };
		// Feed into clippedTriangles2D which was passed in by address
		for (triangle tri : leftAndTopAndRightClippedTriangles)
		{
		std::vector<triangle> clippedTriangles;
		clipByPlane(bottomWallPoint, bottomWallNormal, tri, clippedTriangles);
		trianglesToRasterize.insert(trianglesToRasterize.end(), clippedTriangles.begin(), clippedTriangles.end());
		}
		*/
	}




	void olcEngine3D::drawObject(mesh &myMesh, std::vector<triangle> &trianglesInPaintersOrder)
	{}


	/*

	void olcEngine3D::drawObject(mesh &myMesh, std::vector<triangle> &trianglesInPaintersOrder)
	{
		for (triangle tri : myMesh.tris)
		{
			triangle triProjected, triTranslated;
			triTranslated = rotationTranslationMatrix * tri;
			triTranslated = matNewCoordsToOriginal * triTranslated;

			vec3d scalingVector{ 0.5f * (float)(ScreenWidth()), 0.5f*(float)(ScreenHeight()), 1.0f };
			vec3d lightDirection{ 0.0f, 0.0f, -1.0f };
			float lightDirectionLength = sqrtf(dotProduct(lightDirection, lightDirection));
			lightDirection = lightDirection / lightDirectionLength;

			vec3d normal = normalVectorToTriangle(triTranslated);
			float length = sqrtf(dotProduct(normal, normal));

			if (length > 0)
				normal = normal / length;

			if (dotProduct(normal, triTranslated.p[0] - cameraLocation) < 0)
			{
				triProjected = matProj * (triTranslated - cameraLocation);
				vec3d shiftIntoView{ 1.0f, 1.0f, 0.0f };
				triProjected = triProjected + shiftIntoView;
				componentwiseMultiply(scalingVector, triProjected);

				float naiveLuminosity = dotProduct(normal, lightDirection);
				CHAR_INFO triangleColor = GetColour(naiveLuminosity);

				triProjected.symbol = triangleColor.Char.UnicodeChar;
				triProjected.color = triangleColor.Attributes;

				trianglesInPaintersOrder.push_back(triProjected);
			}

		}
	}

	*/
	
	void olcEngine3D::updateViewMatrices(float fElapsedTime)
	{
		mat4x4 yawRotationMatrix = makeRotationMatrixY(fYaw);
		vec3d upDirection{ 0.0f, 1.0f, 0.0f };
		vec3d target{ 0.0f, 0.0f, 1.0f };
		vec3d lookDirection = yawRotationMatrix * target;
		target = lookDirection + cameraLocation;
		mat4x4 matOriginalToNewCoords = matrixPointAt(cameraLocation, target, upDirection);
		matNewCoordsToOriginal = quickInverse(matOriginalToNewCoords);
		rotationTranslationMatrix = makeRotTranMat(fElapsedTime);
	}


	void olcEngine3D::drawObjects(std::vector<mesh> &myObjects, std::vector<triangle> &trianglesInPaintersOrder)
	{
		for (auto obj : myObjects)
		{
			drawObject(obj, trianglesInPaintersOrder);
		}
	}

	
	std::list<triangle> olcEngine3D::collectAllTrianglesInOnePlace()
	{
		std::list<triangle> result;
		for (mesh &myMesh : myObjects)
		{
			for (triangle tri : myMesh.tris)
			{
				result.push_back(tri);
			}
		}
		return result;
	}


	void olcEngine3D::translateAndRotateToCameraSpace(std::list<triangle> &ourTriangles)
	{
		for (triangle &tri : ourTriangles)
		{
			tri = rotationTranslationMatrix * tri;
			tri = matNewCoordsToOriginal * tri;
		}
	}


	void olcEngine3D::cullAndApplyLighting(std::list<triangle> &ourTriangles)
	{
		vec3d lightDirection{ 0.0f, 0.0f, -1.0f };	
	
		for (std::list<triangle>::iterator i = ourTriangles.begin(); i != ourTriangles.end(); )
		{			
			triangle &tri = *i;
			vec3d normal = normalVectorToTriangle(tri);
			float length = sqrtf(dotProduct(normal, normal));

			if (length > 0)
				normal = normal / length;

			if (dotProduct(normal, tri.p[0] - cameraLocation) >= 0)				// Here's our culling
			{
				i = ourTriangles.erase(i);
			}
			else 
			{
				float naiveLuminosity = dotProduct(normal, lightDirection);
				CHAR_INFO triangleColor = GetColour(naiveLuminosity);

				tri.symbol = triangleColor.Char.UnicodeChar;
				tri.color = triangleColor.Attributes;							// Lighting is done

				i++;	
			}				
		}
	}


	void ZClip(std::list<triangle> &triangles)
	{
		for (std::list<triangle>::iterator i = triangles.begin(); i != triangles.end(); )
		{
			triangle &tri = *i;
			float minZ = min(tri.p[0].z, tri.p[1].z, tri.p[2].z);
			if (minZ < 0.1f)
			{
				i = triangles.erase(i);														
				//tri.color = FG_BLUE;
				//i++;
			}
			else i++;
		}
	}



	void olcEngine3D::screenBorderClip(std::list<triangle> &tris)
	{
		vec3d leftWallPoint{ 0.0f, 0.0f, 0.0f };
		vec3d leftWallNormal{ 1.0f, 0.0f, 0.0f };
		vec3d topWallPoint{ 0.0f, 0.0f, 0.0f };
		vec3d topWallNormal{ 0.0f, 1.0f, 0.0f };
		vec3d rightWallPoint{ ScreenWidth() - 1, 0.0f, 0.0f };
		vec3d rightWallNormal{ -1.0f, 0.0f, 0.0f };
		vec3d bottomWallPoint{ 0.0f, ScreenHeight() - 1, 0.0f };
		vec3d bottomWallNormal{ 0.0f, -1.0f, 0.0f };

		clipByPlane(leftWallPoint, leftWallNormal, tris);
		clipByPlane(topWallPoint, topWallNormal, tris);
		clipByPlane(rightWallPoint, rightWallNormal, tris);
		clipByPlane(bottomWallPoint, bottomWallNormal, tris);
	}


	std::vector<triangle> olcEngine3D::convertListToVector(std::list<triangle> &tris)
	{
		std::vector<triangle> result;
		for (triangle &tri : tris)
			result.push_back(tri);
		return result;
	}


	void olcEngine3D::sortTrianglesByZCoord(std::vector<triangle> &tris)
	{
		std::sort(tris.begin(), tris.end(),
			[](triangle &t1, triangle &t2) {return (t1.p[0].z + t1.p[1].z + t1.p[2].z > t2.p[0].z + t2.p[1].z + t2.p[2].z); }
		);
	}


	bool olcEngine3D::OnUserUpdate(float fElapsedTime)
	{
		clearScreen();
		updateViewMatrices(fElapsedTime);

		std::list<triangle> allTrianglesUnprocessed = collectAllTrianglesInOnePlace();
		translateAndRotateToCameraSpace(allTrianglesUnprocessed);
		cullAndApplyLighting(allTrianglesUnprocessed);		// We put these into one function because they both use the normal vector.
		
		ZClip(allTrianglesUnprocessed);		
		projectTrianglesToScreen(allTrianglesUnprocessed);		
		screenBorderClip(allTrianglesUnprocessed);

		std::vector<triangle> tempTriangleVecForSorting = convertListToVector(allTrianglesUnprocessed);
		sortByZComponent(tempTriangleVecForSorting);
		drawAllTriangles(tempTriangleVecForSorting, FALSE);

		return true;
	}

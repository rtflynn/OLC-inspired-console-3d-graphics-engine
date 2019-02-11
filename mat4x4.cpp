#include "mat4x4.h"


vec3d operator*(mat4x4 const &m, vec3d const &i)
{
	// NOTE:  We usually wouldn't multiply a 4x4 matrix by a 3-dim vector.  It's possible here because
	// we're adding a temporary 4th dimension to the vector and normalizing it after
	// i.e. projectivizing by the new variable w.

	vec3d result;

	result.x = i.x * m.m[0][0] + i.y * m.m[1][0] + i.z * m.m[2][0] + i.proj * m.m[3][0];
	result.y = i.x * m.m[0][1] + i.y * m.m[1][1] + i.z * m.m[2][1] + i.proj * m.m[3][1];
	result.z = i.x * m.m[0][2] + i.y * m.m[1][2] + i.z * m.m[2][2] + i.proj * m.m[3][2];

	float w = i.x * m.m[0][3] + i.y * m.m[1][3] + i.z * m.m[2][3] + i.proj * m.m[3][3];		// If we end up reverting these changes, this last one keeps the m.m[3][3] just times 1

//	if (w != 0) {	
//		result = result / abs(w);
//		return result;
//	}

	if (w > 0) {
		result = result / w;
		return result;
	}
	else if (w < 0) {			// This may break some things regarding clipping.  For some reason dividing by abs(w) 
								// retains the undesired artefacts.
		return result;
	}	
	else {
		std::cout << "Dividing by zero there, bub?   1010101010" << std::endl;
		return result;
	}
}



mat4x4 operator*(mat4x4 const &m1, mat4x4 const &m2)
{
	mat4x4 result{ 0.0f };
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			for (int k = 0; k < 4; k++)
			{
				result.m[i][j] += m1.m[i][k] * m2.m[k][j];
			}

	return result;
}





mat4x4 makeProjectionMatrix(float fAspectRatio, float fNear, float fFar, float fFov)
{
	mat4x4 matProj{ 0.0f };
	//float fNear = 0.1f;
	//float fFar = 1000.0f;
	//float fFov = 90.0f;
	//float fAspectRatio = (float)ScreenHeight() / (float)ScreenWidth();
	float fFovRad = 1.0f / tanf(fFov * 0.5f / 180.0f * 3.14159f);
	float q = fFar / (fFar - fNear);

	matProj.m[0][0] = fAspectRatio * fFovRad;
	matProj.m[1][1] = fFovRad;
	matProj.m[2][2] = q;
	matProj.m[3][2] = -fNear * q;
	matProj.m[2][3] = 1.0f;
	matProj.m[3][3] = 0.0f;

	return matProj;
}




mat4x4 makeRotationMatrixZ(float fTheta)
{
	mat4x4 matRotZ{ 0.0f };
	matRotZ.m[0][0] = cosf(fTheta);
	matRotZ.m[0][1] = sinf(fTheta);
	matRotZ.m[1][0] = -sinf(fTheta);
	matRotZ.m[1][1] = cosf(fTheta);
	matRotZ.m[2][2] = 1.0f;
	matRotZ.m[3][3] = 1.0f;
	return matRotZ;
}





mat4x4 makeRotationMatrixX(float fTheta)
{
	mat4x4 matRotX{ 0.0f };
	matRotX.m[0][0] = 1.0f;
	matRotX.m[1][1] = cosf(fTheta*0.5f);
	matRotX.m[1][2] = sinf(fTheta*0.5f);
	matRotX.m[2][1] = -sinf(fTheta*0.5f);
	matRotX.m[2][2] = cosf(fTheta*0.5f);
	matRotX.m[3][3] = 1.0f;
	return matRotX;
}



mat4x4 makeRotationMatrixY(float fTheta)
{
	mat4x4 matRotY{ 0.0f };
	matRotY.m[0][0] = cosf(fTheta*0.5f);
	matRotY.m[1][1] = 1.0f;
	matRotY.m[0][2] = sinf(fTheta*0.5f);
	matRotY.m[2][0] = -sinf(fTheta*0.5f);
	matRotY.m[2][2] = cosf(fTheta*0.5f);
	matRotY.m[3][3] = 1.0f;
	return matRotY;
}


mat4x4 matrixPointAt(vec3d &pos, vec3d &target, vec3d &up) // Sitting at pos and looking at target
{
	vec3d newForward = target - pos;
	normalize(newForward);

	vec3d projectionUpOntoNewForward = dotProduct(up, newForward) * newForward;
	vec3d newUp = up - projectionUpOntoNewForward;
	normalize(newUp);

	vec3d newRight = crossProduct(newUp, newForward);

	mat4x4 answer{ 0.0f };
	answer.m[0][0] = newRight.x;
	answer.m[0][1] = newRight.y;
	answer.m[0][2] = newRight.z;
	answer.m[1][0] = newUp.x;
	answer.m[1][1] = newUp.y;
	answer.m[1][2] = newUp.z;
	answer.m[2][0] = newForward.x;
	answer.m[2][1] = newForward.y;
	answer.m[2][2] = newForward.z;
	answer.m[3][0] = pos.x;
	answer.m[3][1] = pos.y;
	answer.m[3][2] = pos.z;
	answer.m[3][3] = 1.0f;

	return answer;
}


mat4x4 quickInverse(mat4x4 const &m)		// Works because m has determinant 1 and 0's on rightmost column off the diagonal
{
	mat4x4 inv{ 0.0f };
	inv.m[0][0] = m.m[0][0];
	inv.m[1][0] = m.m[0][1];
	inv.m[2][0] = m.m[0][2];
	inv.m[3][0] = -(m.m[3][0] * m.m[0][0] + m.m[3][1] * m.m[1][0] + m.m[3][2] * m.m[2][0]);
	inv.m[0][1] = m.m[1][0];
	inv.m[1][1] = m.m[1][1];
	inv.m[2][1] = m.m[1][2];
	inv.m[3][1] = -(m.m[3][0] * m.m[0][1] + m.m[3][1] * m.m[1][1] + m.m[3][2] * m.m[2][1]);
	inv.m[0][2] = m.m[2][0];
	inv.m[1][2] = m.m[2][1];
	inv.m[2][2] = m.m[2][2];
	inv.m[3][2] = -(m.m[3][0] * m.m[0][2] + m.m[3][1] * m.m[1][2] + m.m[3][2] * m.m[2][2]);
	inv.m[3][3] = 1.0f;


	return inv;

}




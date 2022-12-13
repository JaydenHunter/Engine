#pragma once
#include <glm/gtc/type_ptr.hpp>
#include <assimp/matrix4x4.h>

namespace MathExt
{
	float* GlmMat4To16f(glm::mat4 matrix)
	{
		float fArray[16];

		const float* pSource = (const float*)glm::value_ptr(matrix);
		for (int i = 0; i < 16; i++)
			fArray[i] = pSource[i];

		return fArray;
	}

	float* GlmVec3To3f(glm::vec3 vec3)
	{
		float fArray[3];

		const float* pSource = (const float*)glm::value_ptr(vec3);
		for (int i = 0; i < 3; i++)
			fArray[i] = pSource[i];

		return fArray;
	}
}


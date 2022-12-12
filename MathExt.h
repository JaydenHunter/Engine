#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

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

}


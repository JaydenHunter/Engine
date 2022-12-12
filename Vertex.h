#pragma once
#include <glm/ext/vector_float3.hpp>
#include <glm/ext/vector_float2.hpp>

#define MAX_BONE_INFLUENCE 4

struct Vertex
{
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 UV;
	glm::vec3 Tangent;
	glm::vec3 Bitangent;

	// Bone indexes which will influence this vertex
	int BoneIDs[MAX_BONE_INFLUENCE];
	// weights from each bone
	float BoneWeights[MAX_BONE_INFLUENCE];
};
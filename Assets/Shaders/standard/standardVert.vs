#version 330 core
layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aUVs;
layout (location = 3) in vec3 aTangent;
layout (location = 4) in vec3 aBitangent;
layout (location = 5) in ivec4 aBoneIDs;
layout (location = 6) in vec4 aBoneWeights;

out vec3 Normal;
out vec3 FragPos;
out vec2 UVs;
out vec3 Tangent;
out vec3 Bitangent;
out ivec4 BoneIDs;
out vec4 BoneWeights;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat3 normalMatrix;

void main()
{
	gl_Position = projection * view * model * vec4(aPosition,1.0);
	FragPos = vec3(model * vec4(aPosition, 1.0));
	Normal = normalMatrix * aNormal;
	UVs = aUVs;
	Tangent = aTangent;
	Bitangent = aBitangent;
	BoneIDs = aBoneIDs;
	BoneWeights = aBoneWeights;
}
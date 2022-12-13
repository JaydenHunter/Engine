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
out mat3 TBN;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat3 normalMatrix;

const int MAX_BONES = 100;
const int MAX_BONE_INFLUENCE = 4;
uniform mat4 finalBonesMatrices[MAX_BONES];

void main()
{

	// Work out the total position of the vertex
	vec4 totalPosition = vec4(0.0f);
	for(int i = 0; i < MAX_BONE_INFLUENCE;i++)
	{
		if(aBoneIDs[i] == -1)
			continue;
		if(aBoneIDs[i] >= MAX_BONES)
		{
			//totalPosition = vec4(aPosition, 1.0f);
			break;
		}
		
		vec4 localPosition = finalBonesMatrices[aBoneIDs[i]] * vec4(aPosition, 1.0f);
		totalPosition += localPosition * aBoneWeights[i];
		vec3 localNormal = mat3(finalBonesMatrices[aBoneIDs[i]]) * aNormal;
	}

	gl_Position = projection * view * model * totalPosition;
	FragPos = vec3(model *totalPosition);
	Normal = normalMatrix * aNormal;
	UVs = aUVs;
	Tangent = aTangent;
	Bitangent = aBitangent;
	BoneIDs = aBoneIDs;
	BoneWeights = aBoneWeights;
	
	// Calculate TBN matrix
	vec3 T = normalize(vec3(model * vec4(aTangent, 0.0)));
	vec3 N = normalize(vec3(model * vec4(aNormal, 0.0)));
	vec3 B = cross(N,T);
	TBN = mat3(T,B,N);
}
#include "Animation.h"
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>

// Need to stop including this
glm::mat4 AssimpMat4ToGlmMat4(aiMatrix4x4 data)
{
	glm::mat4 convertedData;

	convertedData[0][0] = data.a1;
	convertedData[0][1] = data.b1;
	convertedData[0][2] = data.c1;
	convertedData[0][3] = data.d1;

	convertedData[1][0] = data.a2;
	convertedData[1][1] = data.b2;
	convertedData[1][2] = data.c2;
	convertedData[1][3] = data.d2;

	convertedData[2][0] = data.a3;
	convertedData[2][1] = data.b3;
	convertedData[2][2] = data.c3;
	convertedData[2][3] = data.d3;

	convertedData[3][0] = data.a4;
	convertedData[3][1] = data.b4;
	convertedData[3][2] = data.c4;
	convertedData[3][3] = data.d4;

	return convertedData;

}

Animation::Animation(const std::string& animationPath, Model* model)
{
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(animationPath, aiProcess_Triangulate);
	assert(scene && scene->mRootNode);

	// Change this to get all animations later on
	auto animation = scene->mAnimations[0];
	duration = animation->mDuration;
	ticksPerSecond = animation->mTicksPerSecond;
	ReadHeirarchyData(rootNode, scene->mRootNode);
	ReadMissingBones(animation, *model);
}

Bone* Animation::FindBone(const std::string& name)
{
	auto iter = std::find_if(bones.begin(), bones.end(), [&](const Bone& bone)
		{
			return bone.GetBoneName() == name;
		}
	);

	if (iter == bones.end())
		return nullptr;
	else
		return &(*iter);
}

void Animation::ReadMissingBones(const aiAnimation* animation, Model& model)
{
	int size = animation->mNumChannels;

	auto& boneMap = model.GetBoneDataMap();
	int& boneCount = model.GetBoneCount();

	for (unsigned int i = 0; i < size; i++)
	{
		auto channel = animation->mChannels[i];
		std::string boneName = channel->mNodeName.data;

		if (boneMap.find(boneName) == boneMap.end())
		{
			boneMap[boneName].ID = boneCount;
			boneCount++;
		}
		bones.push_back(Bone(channel->mNodeName.data, boneMap[channel->mNodeName.data].ID, channel));
	}

	boneDataMap = boneMap;
}

void Animation::ReadHeirarchyData(AssimpNodeData& dest, const aiNode* src)
{
	assert(src);

	dest.Name = src->mName.data;
	dest.Transformation = Model::AssimpMat4ToGlmMat4(src->mTransformation);
	dest.ChildrenCount = src->mNumChildren;

	for (unsigned int i = 0; i < src->mNumChildren; i++)
	{
		AssimpNodeData newData;
		ReadHeirarchyData(newData, src->mChildren[i]);
		dest.Children.push_back(newData);
	}
}


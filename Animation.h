#pragma once
#include <string>
#include <vector>
#include "Model.h"

struct AssimpNodeData
{
	glm::mat4 Transformation;
	std::string Name;
	int ChildrenCount;
	std::vector<AssimpNodeData> Children;
};

class Animation
{
public:
	Animation() = default;

	Animation(const std::string& animationPath, Model* model);

	~Animation(){}

	Bone* FindBone(const std::string& name);

	inline float GetTicksPerSecond() { return ticksPerSecond; }
	inline float GetDuration() { return duration; }
	inline const AssimpNodeData& GetRoodNode() { return rootNode; }
	inline const std::map<std::string, BoneData>& GetBoneIDMap() { return boneDataMap; }

private:

	void ReadMissingBones(const aiAnimation* animation, Model& model);
	void ReadHeirarchyData(AssimpNodeData& dest, const aiNode* src);

	float duration;
	int ticksPerSecond;
	std::vector<Bone> bones;
	AssimpNodeData rootNode;
	std::map<std::string, BoneData> boneDataMap;
};


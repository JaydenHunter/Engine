#include "Animator.h"

Animator::Animator(Animation* animation)
{
	currentTime = 0.0f;
	currentAnimation = animation;
	finalBoneMatrices.reserve(100);

	for (int i = 0; i < 100; i++)
		finalBoneMatrices.push_back(glm::mat4(1.0f));
}

void Animator::UpdateAnimation(float deltaTime)
{
	this->deltaTime;
	if (currentAnimation)
	{
		currentTime += currentAnimation->GetTicksPerSecond() * deltaTime;
		currentTime = fmod(currentTime, currentAnimation->GetDuration());
		CalculateBoneTransform(&currentAnimation->GetRoodNode(), glm::mat4(1.0f));
	}
}

void Animator::PlayAnimation(Animation* animation)
{
	currentAnimation = animation;
	currentTime = 0.0f;
}

void Animator::CalculateBoneTransform(const AssimpNodeData* node, glm::mat4 parentTransform)
{
	std::string nodeName = node->Name;
	glm::mat4 nodeTransform = node->Transformation;

	Bone* bone = currentAnimation->FindBone(nodeName);

	if (bone)
	{
		bone->Update(currentTime);
		nodeTransform = bone->GetLocalTransform();
	}

	glm::mat4 globalTransformation = parentTransform * nodeTransform;

	auto boneDataMap = currentAnimation->GetBoneIDMap();
	if (boneDataMap.find(nodeName) != boneDataMap.end())
	{
		int index = boneDataMap[nodeName].ID;
		glm::mat4 offset = boneDataMap[nodeName].Offset;
		finalBoneMatrices[index] = globalTransformation * offset;
	}

	for (unsigned int i = 0; i < node->ChildrenCount; i++)
		CalculateBoneTransform(&node->Children[i], globalTransformation);
}

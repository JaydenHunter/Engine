#pragma once
#include "Animation.h"
class Animator
{
public:
	Animator(Animation* animation);

	void UpdateAnimation(float deltaTime);
	void PlayAnimation(Animation* animation);
	void CalculateBoneTransform(const AssimpNodeData* node, glm::mat4 parentTransform);

	std::vector<glm::mat4> GetFinalBoneMatrices() { return finalBoneMatrices; }

private:
	std::vector<glm::mat4> finalBoneMatrices;
	Animation* currentAnimation;
	float currentTime;
	float deltaTime;
};


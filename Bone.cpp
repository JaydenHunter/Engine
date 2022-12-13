#include "Bone.h"
#include <glm/gtx/quaternion.hpp>
Bone::Bone(const std::string& name, int id, const aiNodeAnim* channel)
	: Name(name), ID(id), LocalTransform(1.0f)
{
	NumPositions = channel->mNumPositionKeys;

	for (unsigned int positionIndex = 0; positionIndex < NumPositions; positionIndex++)
	{
		aiVector3D aiPosition = channel->mPositionKeys[positionIndex].mValue;
		float timeStamp = channel->mPositionKeys[positionIndex].mTime;

		KeyPosition data;
		data.Position = glm::vec3(aiPosition.x, aiPosition.y, aiPosition.z);
		data.TimeStamp = timeStamp;
		Positions.push_back(data);
	}

	NumRotations = channel->mNumRotationKeys;

	for (unsigned int rotationIndex = 0; rotationIndex < NumRotations; rotationIndex++)
	{
		aiQuaternion aiOrientation = channel->mRotationKeys[rotationIndex].mValue;
		float timeStamp = channel->mRotationKeys[rotationIndex].mTime;

		KeyRotation data;
		data.Orientation = glm::quat(aiOrientation.w, aiOrientation.x, aiOrientation.y, aiOrientation.z);
		data.TimeStamp = timeStamp;
		Rotations.push_back(data);
	}

	NumScalings = channel->mNumScalingKeys;

	for (unsigned int keyIndex = 0; keyIndex < NumScalings; keyIndex++)
	{
		aiVector3D scale = channel->mScalingKeys[keyIndex].mValue;
		float timeStamp = channel->mScalingKeys[keyIndex].mTime;

		KeyScale data;

		data.Scale = glm::vec3(scale.x, scale.y, scale.z);
		data.TimeStamp = timeStamp;
		Scales.push_back(data);
	}
}

void Bone::Update(float animationTime)
{
	glm::mat4 translation = InterpolatePosition(animationTime);
	glm::mat4 rotation = InterpolateRotation(animationTime);
	glm::mat4 scale = InterpolateScaling(animationTime);
	LocalTransform = translation * rotation * scale;
}

int Bone::GetPositionIndex(float animationTime)
{
	for (unsigned int index = 0; index < NumPositions - 1; index++)
	{
		if (animationTime < Positions[index + 1].TimeStamp)
			return index;
	}
	assert(0);
}

int Bone::GetRotationIndex(float animationTime)
{
	for (unsigned int index = 0; index < NumRotations - 1; index++)
	{
		if (animationTime < Rotations[index + 1].TimeStamp)
			return index;
	}
	assert(0);
}

int Bone::GetScaleIndex(float animationTime)
{
	for (unsigned int index = 0; index < NumScalings - 1; index++)
	{
		if (animationTime < Scales[index + 1].TimeStamp)
			return index;
	}
	assert(0);
}

float Bone::GetScaleFactor(float lastTimeStamp, float nextTimeStamp, float animationTime)
{
	float scaleFactor = 0.0f;
	float midWayLength = animationTime - lastTimeStamp;
	float framesDiff = nextTimeStamp - lastTimeStamp;
	scaleFactor = midWayLength / framesDiff;
	return scaleFactor;

}

glm::mat4 Bone::InterpolatePosition(float animationTime)
{
	if (1 == NumPositions)
		return glm::translate(glm::mat4(1.0f), Positions[0].Position);

	int p0Index = GetPositionIndex(animationTime);
	int p1Index = p0Index + 1;

	float scaleFactor = GetScaleFactor(Positions[p0Index].TimeStamp, Positions[p1Index].TimeStamp, animationTime);
	glm::vec3 finalPosition = glm::mix(Positions[p0Index].Position, Positions[p1Index].Position, scaleFactor);
	return glm::translate(glm::mat4(1.0f), finalPosition);
}

glm::mat4 Bone::InterpolateRotation(float animationTime)
{
	if (1 == NumRotations)
	{
		auto rotation = glm::normalize(Rotations[0].Orientation);
		return glm::toMat4(rotation);
	}

	int p0Index = GetRotationIndex(animationTime);
	int p1Index = p0Index + 1;

	float scaleFactor = GetScaleFactor(Rotations[p0Index].TimeStamp, Rotations[p1Index].TimeStamp, animationTime);
	glm::quat finalRotation = glm::slerp(Rotations[p0Index].Orientation, Rotations[p1Index].Orientation, scaleFactor);
	finalRotation = glm::normalize(finalRotation);
	return glm::toMat4(finalRotation);
}

glm::mat4 Bone::InterpolateScaling(float animationTime)
{
	if (1 == NumScalings)
		return glm::scale(glm::mat4(1.0f), Scales[0].Scale);

	int p0Index = GetScaleIndex(animationTime);
	int p1Index = p0Index + 1;

	float scaleFactor = GetScaleFactor(Scales[p0Index].TimeStamp, Scales[p1Index].TimeStamp, animationTime);

	glm::vec3 finalScale = glm::mix(Scales[p0Index].Scale, Scales[p1Index].Scale, scaleFactor);

	return glm::scale(glm::mat4(1.0f), finalScale);
}

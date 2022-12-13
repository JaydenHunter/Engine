#pragma once
#include <vector>
#include <string>
#include <assimp/anim.h>

#include <glm/ext/matrix_float4x4.hpp>
#include <glm/ext/quaternion_float.hpp>
struct BoneData
{
	// Id of the bone in our Bone Matrices
	int ID;

	// Offset matrix transforms vertex from model space to bone space
	glm::mat4 Offset;
};

struct KeyPosition
{
	glm::vec3 Position;
	float TimeStamp;
};

struct KeyRotation
{
	glm::quat Orientation;
	float TimeStamp;
};

struct KeyScale
{
	glm::vec3 Scale;
	float TimeStamp;
};

class Bone
{
private:
	std::vector<KeyPosition> Positions;
	std::vector<KeyRotation> Rotations;
	std::vector<KeyScale> Scales;

	int NumPositions;
	int NumRotations;
	int NumScalings;

	glm::mat4 LocalTransform;
	std::string Name;
	int ID;

public:
	Bone(const std::string& name, int id, const aiNodeAnim* channel);

	/// <summary>
	/// Interpolates between positions, rotations and scaling keys based on the current time
	/// of the animation and prepares the local transformation matrix by combining all keys transformations.
	/// </summary>
	/// <param name="animationTime"></param>
	void Update(float animationTime);
	
	glm::mat4 GetLocalTransform() { return LocalTransform; }
	std::string GetBoneName() const { return Name; }
	int GetBoneID() { return ID; }

	/// <summary>
	/// Gets the current index on KeyPositions based on the current animation time.
	/// </summary>
	/// <param name="animationTime"></param>
	/// <returns></returns>
	int GetPositionIndex(float animationTime);
	/// <summary>
	/// Gets the current index on KeyRotations based on current animation time.
	/// </summary>
	/// <param name="animationTime"></param>
	/// <returns></returns>
	int GetRotationIndex(float animationTime);
	/// <summary>
	/// Gets the current index on KeyScales based on current animation time.
	/// </summary>
	/// <param name="animationTime"></param>
	/// <returns></returns>
	int GetScaleIndex(float animationTime);

private:

	/// <summary>
	///  Gets Normalized Value for Lerp and Slerp.
	/// </summary>
	/// <param name="lastTimeStep"></param>
	/// <param name="nextTimeStep"></param>
	/// <param name="animationTime"></param>
	/// <returns></returns>
	float GetScaleFactor(float lastTimeStep, float nextTimeStep, float animationTime);

	/// <summary>
	/// Figures out which position keys to interpolate between and performs the interpolation, returning the translation matrix.
	/// </summary>
	/// <param name="animationTime"></param>
	/// <returns></returns>
	glm::mat4 InterpolatePosition(float animationTime);

	/// <summary>
	/// Figures out which rotation keys to interpolate between and performs the interpolation, returning the rotation matrix.
	/// </summary>
	/// <param name="animationTime"></param>
	/// <returns></returns>
	glm::mat4 InterpolateRotation(float animationTime);

	/// <summary>
	/// Figures out which scaling keys to interpolate between and performs the interpolation, returning the scale matrix.
	/// </summary>
	/// <param name="animationTime"></param>
	/// <returns></returns>
	glm::mat4 InterpolateScaling(float animationTime);

};
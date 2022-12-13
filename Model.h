#pragma once
#include "Shader.h"
#include <vector>
#include "Mesh.h"
#include <assimp/scene.h>
#include <map>
#include "Bone.h"



///
class Model
{
public:

	// Model Data
	// Move this to it's own texture manager class
	vector<Texture> Textures_loaded;
	vector<Mesh> Meshes;
	string Directory;
	bool GammaCorrection;

	Model(string const &path, bool gamma = false) : GammaCorrection(gamma)
	{
		LoadModel(path);
	}

	void Draw(Shader& shader);
	auto& GetBoneDataMap() { return boneDataMap; }
	int& GetBoneCount() { return boneCounter; }

	static glm::mat4 AssimpMat4ToGlmMat4(aiMatrix4x4 data);

private:

	// Bone Data (This could be in a Skinned Model Class that inherits from model instead
	map<string, BoneData> boneDataMap;
	int boneCounter = 0;

	

	void SetVertexBoneDataToDefault(Vertex& vertex);
	void SetVertexBoneData(Vertex& vertex, int boneID, float weight);

	void ExtractBoneWeightForVertices(vector<Vertex>& vertices, aiMesh* mesh, const aiScene* scene);

	void LoadModel(string path);
	void ProcessNode(aiNode* node, const aiScene* scene);
	Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene);
	

	vector<Texture> LoadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName);
	unsigned int TextureFromFile(const char* path, const string& directory, bool gamma = false);
	
};



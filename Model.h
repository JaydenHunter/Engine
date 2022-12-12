#pragma once
#include "Shader.h"
#include <vector>
#include "Mesh.h"
#include <assimp/scene.h>




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

private:
	void LoadModel(string path);
	void ProcessNode(aiNode* node, const aiScene* scene);
	Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene);
	

	vector<Texture> LoadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName);
	unsigned int TextureFromFile(const char* path, const string& directory, bool gamma = false);
};


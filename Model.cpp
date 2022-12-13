#include "Model.h"
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include "stb_image.h"

void Model::Draw(Shader& shader)
{
	for (unsigned int i = 0; i < Meshes.size(); i++)
		Meshes[i].Draw(shader);
}

void Model::SetVertexBoneDataToDefault(Vertex& vertex)
{
	for (unsigned int i = 0; i < MAX_BONE_INFLUENCE; i++)
	{
		vertex.BoneIDs[i] = -1;
		vertex.BoneWeights[i] = 0.0f;
	}
}

void Model::SetVertexBoneData(Vertex& vertex, int boneID, float weight)
{
	for (unsigned int i = 0; i < MAX_BONE_INFLUENCE; i++)
	{
		if (vertex.BoneIDs[i] < 0)
		{
			vertex.BoneIDs[i] = boneID;
			vertex.BoneWeights[i] = weight;
			break; // Investigate this break.
		}
	}
}

void Model::ExtractBoneWeightForVertices(vector<Vertex>& vertices, aiMesh* mesh, const aiScene* scene)
{
	for (int boneIndex = 0; boneIndex < mesh->mNumBones; boneIndex++)
	{
		int boneID = -1;
		string boneName = mesh->mBones[boneIndex]->mName.C_Str();

		// Check that it isnt currently in the map
		if (boneDataMap.find(boneName) == boneDataMap.end())
		{
			BoneData newBoneData;
			newBoneData.ID = boneCounter;
			// If issue may be columns/rows in conversion
			newBoneData.Offset = AssimpMat4ToGlmMat4(mesh->mBones[boneIndex]->mOffsetMatrix);
			boneDataMap[boneName] = newBoneData;
			boneID = boneCounter;
			boneCounter++;
		}
		else
		{
			boneID = boneDataMap[boneName].ID;
		}

		assert(boneID != -1);

		auto weights = mesh->mBones[boneIndex]->mWeights;
		int numWeights = mesh->mBones[boneIndex]->mNumWeights;

		for (unsigned int weightIndex = 0; weightIndex < numWeights; weightIndex++)
		{
			int vertexID = weights[weightIndex].mVertexId;
			float weight = weights[weightIndex].mWeight;

			assert(vertexID <= vertices.size());
			SetVertexBoneData(vertices[vertexID], boneID, weight);
		}
	}
}

void Model::LoadModel(string path)
{
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		cout << "ERROR::ASSIMP::" << importer.GetErrorString() << endl;
		return;
	}

	Directory = path.substr(0, path.find_last_of('/'));

	ProcessNode(scene->mRootNode, scene);
}

void Model::ProcessNode(aiNode* node, const aiScene* scene)
{
	// Process all the node's meshes (if any)
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		Meshes.push_back(ProcessMesh(mesh, scene));
	}
	// Then do the same for each of it's children
	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		ProcessNode(node->mChildren[i], scene);
	}
}

Mesh Model::ProcessMesh(aiMesh* mesh, const aiScene* scene)
{
	// Data to fill
	vector<Vertex> vertices;
	vector<unsigned int> indices;
	vector<Texture> textures;

	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		Vertex vertex;

		// Default Bone Data
		SetVertexBoneDataToDefault(vertex);

		// Process vertex positions, normals and UVs
		vertex.Position = glm::vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);

		// Normals
		if (mesh->HasNormals())
		{
			vertex.Normal = glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);
		}
		if (mesh->mTextureCoords[0]) // Does the mesh contain Texture Coordinates?
		{
			// UVs
			vertex.UV = glm::vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);

			// Tangent
			vertex.Tangent = glm::vec3(mesh->mTangents[i].x, mesh->mTangents[i].y, mesh->mTangents[i].z);

			// Bitangent
			vertex.Bitangent = glm::vec3(mesh->mBitangents[i].x, mesh->mBitangents[i].y, mesh->mBitangents[i].z);
		}
		else
			vertex.UV = glm::vec2(0.0f, 0.0f);

		vertices.push_back(vertex);
	}

	// Process Indices
	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];

		for (unsigned int j = 0; j < face.mNumIndices; j++)
			indices.push_back(face.mIndices[j]);
	}

	// Process Material
	if (mesh->mMaterialIndex >= 0)
	{
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

		// Diffuse Maps
		vector<Texture> diffuseMaps = LoadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

		// Specular Maps
		vector<Texture> specularMaps = LoadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
		textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());

		// Normal Maps
		vector<Texture> normalMaps = LoadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal");
		textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());

		// Height Maps
		vector<Texture> heightMaps = LoadMaterialTextures(material, aiTextureType_AMBIENT, "texture_height");
		textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());
	}

	// Get Bone Data
	ExtractBoneWeightForVertices(vertices, mesh, scene);

    return Mesh(vertices, indices, textures);
}

vector<Texture> Model::LoadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName)
{
	vector<Texture> textures;
	for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
	{
		aiString str;
		mat->GetTexture(type, i, &str);
		bool skip = false;
		for (unsigned int j = 0; j < Textures_loaded.size(); j++)
		{
			if (strcmp(Textures_loaded[j].path.data(), str.C_Str()) == 0)
			{
				textures.push_back(Textures_loaded[j]);
				skip = true;
				break;
			}
		}
		if (skip)
			continue;


		Texture texture;
		texture.ID = TextureFromFile(str.C_Str(), Directory);
		texture.type = typeName;
		texture.path = str.C_Str();
		textures.push_back(texture);
		Textures_loaded.push_back(texture); // Add to loaded textures;
	}

	return textures;
}

unsigned int Model::TextureFromFile(const char* path, const string& directory, bool gamma)
{
	string fileName = string(path);
	fileName = directory + '/' + fileName;

	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, nrComponents;
	unsigned char* data = stbi_load(fileName.c_str(), &width, &height, &nrComponents, 0);

	if (data)
	{
		GLenum format;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;
		else
			cout << "File Format not supported, number of color components: " << nrComponents << " for image at path: " << path <<endl;

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}
	else
		cout << "Texture failed to load at path: " << path << endl;

	stbi_image_free(data);

	return textureID;
}

glm::mat4 Model::AssimpMat4ToGlmMat4(aiMatrix4x4 data)
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

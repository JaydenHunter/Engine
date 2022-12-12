#pragma once
#include <vector>
#include "Vertex.h"
#include "Texture.h"
#include "Shader.h"
class Mesh
{
public:
	// Mesh Data
	std::vector<Vertex> Vertices;
	std::vector<unsigned int> Indices;
	std::vector<Texture> Textures;
	unsigned int VAO;
	// Constructor
	Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);

	// Public member functions
	void Draw(Shader& shader);

private:

	// Render Data
	unsigned int VBO, EBO;

	void SetupMesh();
};


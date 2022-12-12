#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader
{
public:
	// The program ID
	unsigned int ID;

	// Constructor reads and builds the shader
	Shader(const char* vertexPath, const char* fragmentPath);

	// Use/Activate the shader
	void Use();

	// Utility uniform functions
	void SetBool(const std::string& name, bool value) const;
	void SetInt(const std::string& name, int value) const;
	void SetFloat(const std::string& name, float value) const;
	void SetVec3(const std::string& name, float value1, float value2, float value3) const;
	void SetVec3(const std::string& name, glm::vec3 value);
	void SetVec4(const std::string& name, float value1, float value2, float value3, float value4) const;
	void SetVec4(const std::string& name, glm::vec4 value) const;
	void SetMat3(const std::string& name, const glm::mat3& mat) const;
	void SetMat4(const std::string& name, const glm::mat4& mat) const;
};


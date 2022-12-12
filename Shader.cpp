#include "Shader.h"

Shader::Shader(const char* vertexPath, const char* fragmentPath)
{
	// 1. Retrieve the vertex/fragment source code from filePath
	std::string vertexCode;
	std::string fragmentCode;
	std::ifstream vertShaderFile;
	std::ifstream fragShaderFile;

	// Ensure ifstream objects can throw exceptions:
	vertShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fragShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try
	{
		// open files
		vertShaderFile.open(vertexPath);
		fragShaderFile.open(fragmentPath);
		std::stringstream vertShaderStream, fragShaderStream;

		// read file's buffer contents into streams
		vertShaderStream << vertShaderFile.rdbuf();
		fragShaderStream << fragShaderFile.rdbuf();

		// close file handlers
		vertShaderFile.close();
		fragShaderFile.close();

		// convert stream into string
		vertexCode = vertShaderStream.str();
		fragmentCode = fragShaderStream.str();
	}
	catch (std::ifstream::failure e)
	{
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
	}
	const char* vShaderCode = vertexCode.c_str();
	const char* fShaderCode = fragmentCode.c_str();

	// 2. Compile shaders
	unsigned int vertex, fragment;
	int	success;
	char infoLog[512];

	// Vertex shader
	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vShaderCode, NULL);
	glCompileShader(vertex);

	// Print compile errors if any
	glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		glGetShaderInfoLog(vertex, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	};

	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fShaderCode, NULL);
	glCompileShader(fragment);

	glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		glGetShaderInfoLog(fragment, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	};

	// Shader program
	ID = glCreateProgram();
	glAttachShader(ID, vertex);
	glAttachShader(ID, fragment);
	glLinkProgram(ID);

	// Print linking errors if any
	glGetProgramiv(ID, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(ID, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}

	// delete the shaders as they're linked into our program now and no longer necessary
	glDeleteShader(vertex);
	glDeleteShader(fragment);
}

void Shader::Use()
{
	glUseProgram(ID);
}

void Shader::SetBool(const std::string& name, bool value) const
{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}

void Shader::SetInt(const std::string& name, int value) const
{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::SetFloat(const std::string& name, int value) const
{
	glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::SetVec3(const std::string& name, float value1, float value2, float value3) const
{
	glUniform3f(glGetUniformLocation(ID, name.c_str()), value1, value2, value3);
}

void Shader::SetVec3(const std::string& name, glm::vec3 value)
{
	SetVec3(name, value.x, value.y, value.z);
}

void Shader::SetVec4(const std::string& name, float value1, float value2, float value3, float value4) const
{
	glUniform4f(glGetUniformLocation(ID, name.c_str()), value1, value2, value3, value4);
}

void Shader::SetMat3(const std::string& name, const glm::mat3& mat) const
{
	glUniformMatrix3fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

void Shader::SetMat4(const std::string& name, const glm::mat4& mat) const
{
	glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

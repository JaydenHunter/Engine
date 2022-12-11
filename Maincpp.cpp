#include <glad/glad.h>
#include <glfw3.h>
#include <iostream>
#include <crtdbg.h>
#include "Shader.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

//Temporary
float vertices[] =
{
	0.5f,0.5f,0.0f, // Top right
	0.5f,-0.5f,0.0f, // Bottom right
	-0.5f,-0.5f,0.0f, // Bottom Left
	-0.5f,0.5f,0.0f // Top left
};

unsigned int indices[] = {
	0,1,3, // First Triangle
	1,2,3 // Second Triangle
};

float verticesTwoSeperate[] = {
	// first triangle
	-0.9f, -0.5f, 0.0f,  // left 
	-0.0f, -0.5f, 0.0f,  // right
	-0.45f, 0.5f, 0.0f,  // top 
	// second triangle
	 0.0f, -0.5f, 0.0f,  // left
	 0.9f, -0.5f, 0.0f,  // right
	 0.45f, 0.5f, 0.0f   // top 
};

float firstTriangle[] = {
	// Positions			// Colors
	-0.9f, -0.5f, 0.0f,		1.0f, 0.0f, 0.0f,  // left 
	-0.0f, -0.5f, 0.0f,		0.0f, 1.0f, 0.0f,// right
	-0.45f, 0.5f, 0.0f,		0.0f, 0.0f, 1.0f// top 
};
float secondTriangle[] = {
	// Positions			// Colors
	0.0f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,// left
	0.9f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,// right
	0.45f, 0.5f, 0.0f,   0.0f, 0.0f, 1.0f// top 
};

float texCoords[] =
{
	0.0f, 0.0f, // lower-left corner
	1.0f, 0.0f, // lower-right corner
	0.5f, 1.0f // top-center corner
};

float vertsRectangle[] =
{
	// positions          // colors           // texture coords
	 0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
	 0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
	-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
	-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left 
};

// Callback for when the user resizes the viewport
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

int main()
{
	// Flag to check for memory leaks
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	// Initialize GLFW with version 3.3 in core profile.
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Create a window object
	GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	// Initialize GLAD.
	// pass GLAD the function to load the address of the OpenGL Function pointers.
	// glfwGetProcAddress that defines the correct function based on which OS we're compiling for.
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// Inform OpenGL the size of the renderering window so OpenGL knows how we want to display the data nd co-ords with respect
	// to the window
	// First two params are location
	// Third and fourth are width and height
	// Setting the viewport dimensions smaller than GLFW's dimensions would make all OpenGL rendering be displayed in a smaller window
	// and we could for example display other elements outside of the OpenGL Viewport.
	// Processed coordiants in OpenGL are between -1 and 1.
	glViewport(0, 0, 800, 600);

	//Register callback event
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	//Testing Code to generate a vertex buffer

	//Vertex Buffer Object
	unsigned int VBO, VAO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertsRectangle), vertsRectangle, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(0 * sizeof(float)));
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6* sizeof(float)));
	glEnableVertexAttribArray(2);

	Shader ourFirstShader("Assets/Shaders/simpleVertShader.vs", "Assets/Shaders/simpleFragShader.fs");

	//set stbi to flip on load
	stbi_set_flip_vertically_on_load(true);

	// Generating a texture
	unsigned int texture1;
	glGenTextures(1, &texture1);
	glBindTexture(GL_TEXTURE_2D, texture1);
	// Set the texture wrapping/filtering options 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	
	// Load and generate texture.
	int width, height, nrChannels;
	unsigned char* data = stbi_load("Assets/Images/container.jpg", &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);

	unsigned int texture2;
	glGenTextures(1, &texture2);
	glBindTexture(GL_TEXTURE_2D, texture2);
	// Set the texture wrapping/filtering options 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	data = stbi_load("Assets/Images/awesomeface.png", &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);
	ourFirstShader.Use();
	glUniform1i(glGetUniformLocation(ourFirstShader.ID, "texture1"), 0); // Set it manually
	ourFirstShader.SetInt("texture2", 1); // or set with shader class
	// Render loop
	while (!glfwWindowShouldClose(window))
	{
		// Clear the screen
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// Handle inputs
		processInput(window);
		
		// Rendering commands go here
		//....
		// 5. Draw the object

		//glUniform4f(vertexColorLocation1, 0.0f, alteredValue, 0.0f, 1.0f);
		// Draw the first triangle using the data from the first VAO
		//glBindVertexArray(VAOs[0]);
		//glDrawArrays(GL_TRIANGLES, 0, 3);
		//// then we draw the second triangle using the data from the second VAO
		////glUniform4f(vertexColorLocation1, alteredValue, 0.0f, 0.0f, 1.0f);
		//glBindVertexArray(VAOs[1]);
		//glDrawArrays(GL_TRIANGLES, 0, 3);

		glBindVertexArray(VAO);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		// Check and call events and swap the buffers.
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	/*glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteProgram(shaderProgram);*/
	glfwTerminate();
	return 0;
}


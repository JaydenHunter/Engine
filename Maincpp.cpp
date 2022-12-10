#include <glad/glad.h>
#include <glfw3.h>
#include <iostream>
#include <crtdbg.h>

//Temporary Vertex Shader
const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"layout (location = 1) in vec3 aColor;\n"
"out vec3 ourColor;\n"
"void main()\n"
"{\n"
"gl_Position = vec4(aPos.x,aPos.y,aPos.z,1.0);\n"
"ourColor = aColor;\n"
"}\0";

//Temporary Fragment Shader
const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"in vec3 ourColor;\n"
"void main()\n"
"{\n"
"FragColor = vec4(ourColor,1.0);\n"
"}\0";
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
	unsigned int VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	// Copy the vertex data into the buffer's memory
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// Create our vertex shader
	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	// Check if shader compiled correctly 
	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	else
	{
		std::cout << "SUCCESS::SHADER::VERTEX::COMPILATION_SUCCESS" << std::endl;
	}

	// Create our first fragment shader
	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	// Check if fragment shader compiled correctly
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	else
	{
		std::cout << "SUCCESS::SHADER::FRAGMENT::COMPILATION_SUCCESS" << std::endl;
	}

	// Link our shaders to a shader program which will be used when we issue render calls
	unsigned int shaderProgram;
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	// After linking, we should delete our shaders
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	// Check if the shader program was successful
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER_PROGRAM::LINKING_FAILED" << std::endl;
	}
	else
	{
		std::cout << "SUCCESS::SHADER_PROGRAM::LINKING_SUCCESS" << std::endl;
	}

	// We can generate multiple VAOS or buffers at the same time
	unsigned int VBOs[2], VAOs[2];
	glGenVertexArrays(2, VAOs);
	glGenBuffers(2, VBOs);

	// Set up first triangle data
	glBindVertexArray(VAOs[0]);
	glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(firstTriangle), firstTriangle, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	// We dont need to unbind as we are directly binding to a different VAO
	// Set up second triangle data
	glBindVertexArray(VAOs[1]);
	glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(secondTriangle), secondTriangle, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glBindVertexArray(0); // unbinds the vertex

	//// Creating a VAO - Vertex Array Object
	//unsigned int VAO;
	//glGenVertexArrays(1, &VAO);
	//// Initialization code (Done once (unless your object frequently changes))
	//// 1. Bind Vertex Array Object
	//glBindVertexArray(VAO);
	//// 2. Copy our vertices array in a buffer for OpenGL to use
	//glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(verticesTwoSeperate), verticesTwoSeperate, GL_STATIC_DRAW);

	//// 3. Copy our index array in an elemnt buffer.
	//unsigned int EBO;
	//glGenBuffers(1, &EBO);
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	//// 4. Set out vertex attribute pointers
	//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	//glEnableVertexAttribArray(0);

	// EBO (Element Buffer Objects)

	// Draw as wirefram
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	// Draw normally
	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	// Render loop
	while (!glfwWindowShouldClose(window))
	{
		// Clear the screen
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// Handle inputs
		processInput(window);
		/*float timeValue = glfwGetTime();
		float alteredValue = (sin(timeValue) / 2.0f) + 0.5f;
		int vertexColorLocation1 = glGetUniformLocation(shaderProgram, "vertexColor");*/
		// Rendering commands go here
		//....
		// 5. Draw the object
		//glUseProgram(shaderProgram);
		////glBindVertexArray(VAO);
		//glDrawArrays(GL_TRIANGLES, 0, 6);
		////glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		////glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		glUseProgram(shaderProgram);
		//glUniform4f(vertexColorLocation1, 0.0f, alteredValue, 0.0f, 1.0f);
		// Draw the first triangle using the data from the first VAO
		glBindVertexArray(VAOs[0]);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		// then we draw the second triangle using the data from the second VAO
		//glUniform4f(vertexColorLocation1, alteredValue, 0.0f, 0.0f, 1.0f);
		glBindVertexArray(VAOs[1]);
		glDrawArrays(GL_TRIANGLES, 0, 3);

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


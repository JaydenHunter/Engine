#include <glad/glad.h>
#include <glfw3.h>
#include <iostream>
#include <crtdbg.h>
#include "Shader.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include "Camera.h"
#include "Vendor/imgui.h"
#include "Vendor/imgui_impl_glfw.h"
#include "Vendor/imgui_impl_opengl3.h"
#include "Vendor/ImGuizmo.h"
#include "MathExt.h"
#include "Light.h"
#include "Model.h"

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

unsigned int LoadTexture(char const* path);
void SetShaderDataByLightType(Light light, Shader* shader, int index);

bool editMode = true;
//Temporary
float vertices[] = {
	// positions          // normals           // texture coords
-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
 0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
-0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
};

glm::vec3 cubePositions[] = {
	glm::vec3(0.0f,  0.0f,  0.0f),
	glm::vec3(2.0f,  5.0f, -15.0f),
	glm::vec3(-1.5f, -2.2f, -2.5f),
	glm::vec3(-3.8f, -2.0f, -12.3f),
	glm::vec3(2.4f, -0.4f, -3.5f),
	glm::vec3(-1.7f,  3.0f, -7.5f),
	glm::vec3(1.3f, -2.0f, -2.5f),
	glm::vec3(1.5f,  2.0f, -2.5f),
	glm::vec3(1.5f,  0.2f, -1.5f),
	glm::vec3(-1.3f,  1.0f, -1.5f)
};

// Callback for when the user resizes the viewport
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

float deltaTime = 0.0f;
float lastFrame = 0.0f;
float lastX = 400, lastY = 300;
Camera camera;

bool spaceWasReleased = true;
void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (!editMode)
	{
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
			camera.ProcessKeyboard(CAMERA_MOVEMENT::FORWARD, deltaTime);
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
			camera.ProcessKeyboard(CAMERA_MOVEMENT::BACKWARD, deltaTime);
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
			camera.ProcessKeyboard(CAMERA_MOVEMENT::LEFT, deltaTime);
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
			camera.ProcessKeyboard(CAMERA_MOVEMENT::RIGHT, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS && spaceWasReleased)
	{
		editMode = !editMode;

		if (!editMode)
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		else
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

		spaceWasReleased = false;
	}
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_RELEASE)
		spaceWasReleased = true;

}

bool firstMouse = true;

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xOffset = xpos - lastX;
	float yOffset = lastY - ypos; // reversed since y-coordinates range from bottom to top
	lastX = xpos;
	lastY = ypos;
	if (!editMode)
	{
		camera.ProcessMouseMovement(xOffset, yOffset, true);
	}
}

float fov = 45;

void scroll_callback(GLFWwindow* window, double xOffset, double yOffset)
{
	if (editMode)
		return;

	fov -= (float)yOffset;

	if (fov < 1.0f)
		fov = 1.0f;
	if (fov > 65.0f)
		fov = 65.0f;
}

int main()
{
	//------
	// Flag to check for memory leaks
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	// Initialize GLFW with version 3.3 in core profile.
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Create a window object
	GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "LearnOpenGL", NULL, NULL);
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
	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

	//Register callback events
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
	//Testing Code to generate a vertex buffer

	//Vertex Buffer Object
	unsigned int VBO, VAO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(0 * sizeof(float)));
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	//Shader ourFirstShader("Assets/Shaders/lightVertShader.vs", "Assets/Shaders/lightFragShader.fs");
	Shader ourFirstShader("Assets/Shaders/standard/standardVert.vs", "Assets/Shaders/standard/standardFrag.fs");

	//set stbi to flip on load
	//stbi_set_flip_vertically_on_load(true);



	ourFirstShader.Use();
	//ourFirstShader.SetVec3("objectColor", 1.0f, 0.5f, 0.31f);
	//ourFirstShader.SetVec3("lightColor", 1.0f, 1.0f, 1.0f);
	//ourFirstShader.SetInt("material.diffuse", 0);
	//ourFirstShader.SetInt("material.specular", 1);
	//ourFirstShader.SetInt("material.emission", 2);

	//Scale and rotate our rectangle
	glm::mat4 trans = glm::mat4(1.0f);
	trans = glm::rotate(trans, glm::radians(90.0f), glm::vec3(0.0, 0.0, 0.0));
	trans = glm::scale(trans, glm::vec3(0.5, 0.5, 0.5));

	unsigned int transformLoc = glGetUniformLocation(ourFirstShader.ID, "transform");
	glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));;

	//Enable Depth testing
	glEnable(GL_DEPTH_TEST);

	//temp light
	unsigned int lightVAO;
	glGenVertexArrays(1, &lightVAO);
	glBindVertexArray(lightVAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(0 * sizeof(float)));
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	Shader lightSourceShader("Assets/Shaders/lightVertShader.vs", "Assets/Shaders/simpleWhiteFragShader.fs");


	// IMGUI test
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	ImFont* fontBold = io.Fonts->AddFontFromFileTTF("Assets\\Fonts\\IBM_Plex_Sans\\IBMPlexSans-Bold.ttf", 12.0f);
	ImFont* fontRegular = io.Fonts->AddFontFromFileTTF("Assets\\Fonts\\IBM_Plex_Sans\\IBMPlexSans-Regular.ttf", 12.0f);

	// Setup platform/renderer bindings
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 330");

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(5, 5));
	ourFirstShader.Use();

	static float lightPosition[3] = { 1.2f, 1.0f, 2.0f };
	static float rotation[3] = { 0.0f,0.0f,0.0f };

	float viewManipulateRight = io.DisplaySize.x;
	float viewManipulateTop = 0;

	// Material Properties
	static float specularColor[3] = { 0.5f, 0.5f, 0.5f };
	static float shininess = 32.0f;
	ourFirstShader.SetFloat("material.shininess", shininess);




	Light lights[6];
	// Our Main Spotlight
	lights[0] = Light(Light::SPOTLIGHT);
	lights[0].Ambient = glm::vec3(0.0f, 0.0f, 0.0f);
	lights[0].Specular = glm::vec3(1.0f, 1.0f, 1.0f);
	lights[0].Diffuse = glm::vec3(1.0f, 1.0f, 1.0f);
	lights[0].Linear = 0.09f;
	lights[0].Quadratic = 0.032f;
	lights[0].InnerCutOff = 7.0f;
	lights[0].OuterCutOff = 9.0f;
	// Sunlight
	lights[1] = Light(Light::DIRECTIONAL);
	// Point Lights (4)
	lights[2] = Light(Light::POINT, glm::vec3(0.7f, 0.2f, 2.0f));
	lights[3] = Light(Light::POINT, glm::vec3(2.3f, -3.3f, -4.0f));
	lights[4] = Light(Light::POINT, glm::vec3(-4.0f, 2.0f, -12.0f));
	lights[5] = Light(Light::POINT, glm::vec3(0.0f, 0.0f, -3.0f));

	// Set colors
	lights[1].Ambient = glm::vec3(0.5f * 0.01f, 0.5f * 0.01f, 0.5f * 0.01f);
	lights[1].Diffuse = glm::vec3(0.5f, 0.5f, 0.5f);
	lights[1].Specular = glm::vec3(0.5f, 0.5f, 0.5f);

	lights[2].Ambient = glm::vec3(0.86f * 0.01f, 0.25f * 0.01f, 0.13f * 0.01f);
	lights[2].Diffuse = glm::vec3(0.86f * 0.5f, 0.25f * 0.5f, 0.13f * 0.5f);
	lights[2].Specular = glm::vec3(0.86f * 0.5f, 0.25f * 0.5f, 0.13f * 0.5f);

	lights[3].Ambient = glm::vec3(0.13f * 0.01f, 0.86f * 0.01f, 0.13f * 0.01f);
	lights[3].Diffuse = glm::vec3(0.13f * 0.5f, 0.86f * 0.5f, 0.13f * 0.5f);
	lights[3].Specular = glm::vec3(0.13f * 0.5f, 0.86f * 0.5f, 0.13f * 0.5f);

	lights[4].Ambient = glm::vec3(0.13f * 0.01f, 0.25f * 0.01f, 0.86f * 0.01f);
	lights[4].Diffuse = glm::vec3(0.13f * 0.5f, 0.25f * 0.5f, 0.86f * 0.5f);
	lights[4].Specular = glm::vec3(0.13f * 0.5f, 0.25f * 0.5f, 0.86f * 0.5f);

	lights[5].Ambient = glm::vec3(0.86f * 0.01f, 0.25f * 0.01f, 0.86f * 0.01f);
	lights[5].Diffuse = glm::vec3(0.86f * 0.5f, 0.25f * 0.5f, 0.86f * 0.5f);
	lights[5].Specular = glm::vec3(0.86f * 0.5f, 0.25f * 0.5f, 0.86f * 0.5f);



	// Set our static lights to the shader data
	for (int i = 0; i < 6; i++)
		SetShaderDataByLightType(lights[i], &ourFirstShader, i);

	// OUR FIRST MODEL LOADED
	Model backpack("Assets/Models/Sponza/NewSponza_Main_Yup_002.fbx");

	// These will usually be called each time before you render the cube, but because they arent changing yet, Im leaving it here.
	// Generating a texture
	unsigned int diffuseMap = LoadTexture("Assets/Models/Turret/Turret_Turret_MAT_Diffuse.png");
	unsigned int specularMap = LoadTexture("Assets/Models/Turret/Turret_Turret_MAT_SpecularGlossiness.png");
	unsigned int normalMat = LoadTexture("Assets/Models/Turret/Turret_Turret_MAT_Normal.png");


	ourFirstShader.SetInt("material.texture_diffuse1", 0);
	ourFirstShader.SetInt("material.texture_specular1", 1);
	ourFirstShader.SetInt("material.texture_normal1", 2);

	// Render loop
	while (!glfwWindowShouldClose(window))
	{
		// Calculate Delta Time
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// Handle inputs
		glfwPollEvents();
		processInput(window);

		// Clear the screen
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Feed inputs to dear imgui, start new frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		// View and Projection Matrix
		glm::mat4 view = camera.GetViewMatrix();
		glm::mat4 projection = glm::perspective(glm::radians(fov), io.DisplaySize.x / io.DisplaySize.y, 0.1f, 100.0f);



		// Old code for reference
		{
			////ourFirstShader.SetVec3("light.position")

			//ourFirstShader.Use();
			if (editMode)
			{
				ImGui::PushFont(fontBold);
				ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f));
				ImGui::Begin("Settings", NULL, ImGuiWindowFlags_NoMove);


				//	// END GIZMOS
				//	ImGui::LabelText(" ", "Light Settings");
				//	ImGui::PopFont();

				//	ImGui::PushFont(fontRegular);
				//	ImGui::Spacing();
				//	ImGui::Separator();
				//	ImGui::Spacing();
				//	ImGui::PopFont();

				//	ImGui::PushFont(fontBold);
				//	ImGui::LabelText(" ", "Cube Settings");
				//	ImGui::PopFont();
				//	ImGui::PushFont(fontRegular);
				ImGui::DragFloat3("Rotation", rotation);
				for (int i = 0; i < 3; i++)
				{
					if (rotation[i] < 0)
						rotation[i] = 360;
					if (rotation[i] > 360)
						rotation[i] = 0;
				}

				//	ImGui::ColorEdit3("Specular", specularColor);
				//	//ImGui::SliderFloat("Shininess", &shininess, 0.0f, 1.0f);
				//	ourFirstShader.SetVec3("material.specular", specularColor[0], specularColor[1], specularColor[2]);
					//ourFirstShader.SetFloat("material.shininess", shininess);
				ImGui::PopFont();
				ImGui::End();
			}

			//glm::vec3 lightPosV3 = glm::vec3(lightPosition[0], lightPosition[1], lightPosition[2]);
			////ourFirstShader.SetVec3("light.position", lightPosV3);
			//// Set up shader for coloured cube
			//ourFirstShader.SetMat4("projection", projection);
			//ourFirstShader.SetMat4("view", view);
			//ourFirstShader.SetVec3("viewPos", camera.Position);
			//lights[0].Position = camera.Position;
			//lights[0].Direction = camera.Forward;

			//SetShaderDataByLightType(lights[0], &ourFirstShader, 0);
			//// World Transform

			//for (int i = 0; i < 10; i++)
			//{
				//glm::mat4 model = glm::mat4(1.0f);
				//model = glm::translate(model, cubePositions[i]);

				//ourFirstShader.SetMat4("model", model);
			//	glm::mat3 normalMatrix = model;
			//	normalMatrix = glm::inverseTranspose(normalMatrix);
			//	ourFirstShader.SetMat3("normalMatrix", normalMatrix);

			//	// Render first cube
			//	glBindVertexArray(VAO);
			//	glDrawArrays(GL_TRIANGLES, 0, 36);
			//}

			//// Draw the light object
			//lightSourceShader.Use();
			//lightSourceShader.SetMat4("projection", projection);
			//lightSourceShader.SetMat4("view", view);

			//model = glm::mat4(1.0f);
			//model = glm::translate(model, lightPosV3);
			//model = glm::scale(model, glm::vec3(0.2f));
			//lightSourceShader.SetMat4("model", model);

			//glBindVertexArray(lightVAO);
			//glDrawArrays(GL_TRIANGLES, 0, 36);
		}


		// Rendering our model
		ourFirstShader.Use();
		ourFirstShader.SetMat4("projection", projection);
		ourFirstShader.SetMat4("view", view);
		SetShaderDataByLightType(lights[0], &ourFirstShader, 0);
		ourFirstShader.SetVec3("viewPos", camera.Position);
		lights[0].Position = camera.Position;
		lights[0].Direction = camera.Forward;
		glm::mat4 model = glm::mat4(1.0f);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, diffuseMap);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, specularMap);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, normalMat);
		for (int i = 0; i < 1; i++)
		{
			model = glm::mat4(1.0f);
			//model = glm::translate(model,cubePositions[i] * 1.5f);
			//model = glm::translate(model,cubePositions[i] * 1.5f);
			model = glm::rotate(model, glm::radians(rotation[0]), glm::vec3(1.0f, 0.0f, 0.0f));
			model = glm::rotate(model, glm::radians(rotation[1]), glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::rotate(model, glm::radians(rotation[2]), glm::vec3(0.0f, 0.0f, 1.0f));
			model = glm::scale(model, glm::vec3(0.01f, 0.01f, 0.01f));
			ourFirstShader.SetMat4("model", model);

			glm::mat3 normalMatrix = model;
			normalMatrix = glm::inverseTranspose(normalMatrix);
			ourFirstShader.SetMat3("normalMatrix", normalMatrix);
			backpack.Draw(ourFirstShader);
		}



		

		// Render IMGUI
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		int display_w, display_h;
		glfwGetFramebufferSize(window, &display_w, &display_h);
		glViewport(0, 0, display_w, display_h);
		// Check and call events and swap the buffers.
		glfwSwapBuffers(window);

	}

	/*glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteProgram(shaderProgram);*/
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
	glfwTerminate();
	return 0;
}

unsigned int LoadTexture(char const* path)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, nrComponents;
	unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);
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
		{
			std::cout << "Number of color components not supported for file at path: " << path << std::endl;
			format = GL_RGB;
		}

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}
	else
		std::cout << "Texture failed to load at apath: " << path << std::endl;

	stbi_image_free(data);

	return textureID;
}

void SetShaderDataByLightType(Light light, Shader* shader, int index)
{
	// Move this later and also dynamically set or something.
	std::string indexer = "pointLights[" + std::to_string(index - 2) + "].";
	switch (light.LightType)
	{
	case Light::POINT:
		shader->SetVec3(indexer + "position", light.Position);
		shader->SetVec3(indexer + "ambient", light.Ambient);
		shader->SetVec3(indexer + "diffuse", light.Diffuse);
		shader->SetVec3(indexer + "specular", light.Specular);
		shader->SetFloat(indexer + "constant", light.Constant);
		shader->SetFloat(indexer + "linear", light.Linear);
		shader->SetFloat(indexer + "quadratic", light.Quadratic);
		break;

	case Light::DIRECTIONAL:
		shader->SetVec3("directionalLight.direction", light.Direction);
		shader->SetVec3("directionalLight.ambient", light.Ambient);
		shader->SetVec3("directionalLight.diffuse", light.Diffuse);
		shader->SetVec3("directionalLight.specular", light.Specular);
		break;

	case Light::SPOTLIGHT:
		shader->SetVec3("spotLight.position", light.Position);
		shader->SetVec3("spotLight.direction", light.Direction);
		shader->SetVec3("spotLight.ambient", light.Ambient);
		shader->SetVec3("spotLight.diffuse", light.Diffuse);
		shader->SetVec3("spotLight.specular", light.Specular);
		shader->SetFloat("spotLight.constant", light.Constant);
		shader->SetFloat("spotLight.linear", light.Linear);
		shader->SetFloat("spotLight.quadratic", light.Quadratic);
		shader->SetFloat("spotLight.innerCutOff", glm::cos(glm::radians(light.InnerCutOff)));
		shader->SetFloat("spotLight.outerCutOff", glm::cos(glm::radians(light.OuterCutOff)));
		break;

	default:
		break;
	}
}

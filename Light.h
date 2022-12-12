#pragma once
#include <glm/glm.hpp>
class Light
{
public:

	enum LIGHT_TYPE
	{
		SPOTLIGHT,
		DIRECTIONAL,
		POINT
	};
	// Change this to use a transform component
	// They should also be private
	glm::vec3 Position;
	glm::vec3 Direction;

	// Colors
	glm::vec3 Ambient;
	glm::vec3 Diffuse;
	glm::vec3 Specular;

	// Point Light and Spot Lights
	float Constant;
	float Linear;
	float Quadratic;

	// Spot Light
	float InnerCutOff;
	float OuterCutOff;

	LIGHT_TYPE LightType;

	Light(LIGHT_TYPE lightType = LIGHT_TYPE::POINT, glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 direction = glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3 ambient = glm::vec3(1.0f, 1.0f, 1.0f)
		, glm::vec3 diffuse = glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3 specular = glm::vec3(1.0f, 1.0f, 1.0f), float constant = 1, float linear = 0.09f, float quadratic = 0.032f
		, float innerCutOff = 12.5f, float outerCutOff = 18.0f)
		: LightType(lightType), Position(position), Direction(direction), Ambient(ambient), Diffuse(diffuse), Specular(specular), Constant(constant), Linear(linear), Quadratic(quadratic), InnerCutOff(innerCutOff), OuterCutOff(outerCutOff)
	{

	}
};


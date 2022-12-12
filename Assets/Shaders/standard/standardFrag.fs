#version 330 core
struct Material
{
	sampler2D texture_diffuse1;
	sampler2D texture_diffuse2;
	sampler2D texture_diffuse3;
	sampler2D texture_specular1;
	sampler2D texture_specular2;
};

in vec3 Normal;
in vec3 FragPos;
in vec2 UVs;

out vec4 FragColor;

uniform Material material;
uniform vec3 viewPos;

// add lighting functions

void main()
{
	FragColor = texture(material.texture_diffuse1,UVs);
}
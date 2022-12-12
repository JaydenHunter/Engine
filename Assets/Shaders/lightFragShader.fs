#version 330 core
struct Material
{
	sampler2D diffuse;
	sampler2D specular;
	sampler2D emission;
	float shininess;
};

struct Light
{
	vec4 position;
	
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	
	float constant;
	float linear;
	float quadratic;
};

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;

out vec4 FragColor;



uniform Material material;
uniform Light light;
uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 viewPos;

void main()
{

	// --- Ambient Lighting ---
	vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));
	
	// --- Calculate Diffuse Lighting ----
	vec3 norm = normalize(Normal);
	vec3 lightDir;
	if(light.position.w == 0.0) // Directional light
		lightDir = normalize(-light.position.xyz);
	else // Positional Light
		lightDir = normalize(light.position.xyz - FragPos);
	
	// Get the dot product, clamping the min value to 0 so that we dont go into negatives
	float diff = max(dot(norm,lightDir),0.0);
	vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));
	
	// --- Specular Lighting ---	
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir),0.0), material.shininess);
	vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));
	
	// --- Emission ---
	vec3 emission = vec3(texture(material.emission, TexCoords).rgb);
	
	// --- Attenuation ---
	if(light.position.w > 0.0)
	{
		float distance = length(light.position.xyz - FragPos);
		float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
		ambient *= attenuation;
		diffuse *= attenuation;
		specular *= attenuation;
	}
	// --- Result ---
	vec3 result = ambient + diffuse + specular; // +emission
	FragColor = vec4(result,1.0);
}


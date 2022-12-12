#version 330 core
struct Material
{
	sampler2D texture_diffuse1;
	sampler2D texture_diffuse2;
	sampler2D texture_diffuse3;
	sampler2D texture_specular1;
	sampler2D texture_specular2;
};

struct DirectionalLight
{
	vec3 direction;
	
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

struct PointLight
{
	vec3 position;
	
	float constant;
	float linear;
	float quadratic;
	
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

struct SpotLight
{
	vec3 position;
	vec3 direction;
	
	float innerCutOff;
	float outerCutOff;
	
	float constant;
	float linear;
	float quadratic;
	
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

// --- Function Prototypes ---
vec3 CalculateDirectionalLight(DirectionalLight light, vec3 normal, vec3 viewDirection);
vec3 CalculatePointLight(PointLight light, vec3 normal, vec3 fragPosition, vec3 viewDirection);
vec3 CalculateSpotLight(SpotLight light, vec3 normal, vec3 fragPosition, vec3 viewDirection);

in vec3 Normal;
in vec3 FragPos;
in vec2 UVs;

out vec4 FragColor;

uniform Material material;
uniform vec3 viewPos;

// --- Lights ---
#define NR_POINT_LIGHTS 4
uniform PointLight pointLights[NR_POINT_LIGHTS];
uniform DirectionalLight directionalLight;
uniform SpotLight spotLight;

// Temp
#define shininess = 32.0

// add lighting functions

void main()
{
	// Properties
	vec3 norm = normalize(Normal);
	vec3 viewDirection = normalize(viewPos - FragPos);
	
	// Phase 1: DirectionalLight
	vec3 result = CalculateDirectionalLight(directionalLight, norm, viewDirection);
	
	// Phase 2: Point Lights
	for(int i = 0; i < NR_POINT_LIGHTS; i++)
		result += CalculatePointLight(pointLights[i], norm, FragPos, viewDirection);
	
	// Phase 3: Spot Light
	result += CalculateSpotLight(spotLight, norm, FragPos, viewDirection);
	
	FragColor = vec4(result, 1.0);
}


vec3 CalculateDirectionalLight(DirectionalLight light, vec3 normal, vec3 viewDirection)
{
	vec3 lightDir = normalize(-light.direction);
	
	// Diffuse Shading
	float diff = max(dot(normal, lightDir),0.0);
	
	// Specular Shading
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDirection, reflectDir), 0.0), 32.0);
	
	// Combine Results
	vec3 ambient = light.ambient * vec3(texture(material.texture_diffuse1, UVs));
	vec3 diffuse = light.diffuse * diff * vec3(texture(material.texture_diffuse1, UVs));
	vec3 specular = light.specular * spec * vec3(texture(material.texture_specular1, UVs));
	return (ambient + diffuse + specular);
}

vec3 CalculatePointLight(PointLight light, vec3 normal, vec3 fragPosition, vec3 viewDirection)
{
	vec3 lightDir = normalize(light.position - fragPosition);
	
	// Diffuse Shading
	float diff = max(dot(normal, lightDir), 0.0);
	
	// Specular Shading
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDirection, reflectDir), 0.0), 32.0);
	
	// Attenuation
	float distance = length(light.position = fragPosition);
	float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
	
	// Combine Results
	vec3 ambient = light.ambient * vec3(texture(material.texture_diffuse1, UVs));
	vec3 diffuse = light.diffuse * diff * vec3(texture(material.texture_diffuse1, UVs));
	vec3 specular = light.specular * spec * vec3(texture(material.texture_specular1, UVs));
	
	ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;
	
	return (ambient + diffuse + specular);
}

vec3 CalculateSpotLight(SpotLight light, vec3 normal, vec3 fragPosition, vec3 viewDirection)
{
	vec3 lightDir = normalize(light.position - fragPosition);
	
	// Diffuse Shading
	float diff = max(dot(normal, lightDir), 0.0);
	
	// Specular Shading
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDirection, reflectDir),0.0),32.0);
	
	// Attenuation
	float distance = length(light.position - fragPosition);
	float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
	
	// Spotlight Intensity
	float theta = dot(lightDir, normalize(-light.direction));
	float epsilon = light.innerCutOff - light.outerCutOff;
	float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
	
	// Combine Results
	vec3 ambient = light.ambient * vec3(texture(material.texture_diffuse1, UVs));
	vec3 diffuse = light.diffuse * diff * vec3(texture(material.texture_diffuse1, UVs));
	vec3 specular = light.specular * spec * vec3(texture(material.texture_specular1, UVs));
	
	ambient *= attenuation * intensity;
	diffuse *= attenuation * intensity;
	specular *= attenuation * intensity;
	
	return (ambient + diffuse + specular);
}
#version 330 core
out vec4 FragColor;

in vec3 Normal;

vec3 lightDir = vec3(-0.2f, -0.5f, -0.7f);
vec3 objectColor = vec3(0.0f, 1.0f, 0.0f);
float ambientStrength = 0.2f;

void main()
{
	// Ambient lighting
	vec3 ambient = vec3(ambientStrength);

	// Diffuse lighting
	float diff = max(dot(Normal, normalize(lightDir)), 0.0);
	vec3 diffuse = vec3(diff);

	vec3 finalColor = (ambient + diffuse) * objectColor;
	FragColor = vec4(finalColor, 1.0f);
}
#version 460
#extension GL_ARB_separate_shader_objects : enable

layout (location = 0) in vec3 Normal;
layout (location = 1) in vec3 FragPos;


layout (location = 0) out vec4 outColor;

layout (binding = 1) uniform Light
{
	vec4 Pos;
	vec4 Color;
} light;

void main()
{
	//ambient light
	float ambientStrength = 0.1;
    vec4 ambient = ambientStrength * light.Color;

	//diffuse light
	vec3 norm = normalize(Normal);
	vec4 lightDir = normalize(light.Pos - vec4(FragPos, 1.0));  

	float diff = max(dot(vec4(norm,1.0), lightDir), 0.0);
	vec4 diffuse = diff * light.Color;


	vec4 result = (ambient + diffuse) * vec4(1.0, 1.0, 0.0 , 1.0);
	result.w = 1.0;

	outColor = result;
}
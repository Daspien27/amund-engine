#version 460
#extension GL_ARB_separate_shader_objects : enable

layout (binding = 0) uniform UniformBufferObject 
{
	mat4 proj;
	mat4 view;
	mat4 model;
} ubo;

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inNormal;

layout (location = 0) out vec3 Normal;
layout (location = 1) out vec3 FragPos;

out gl_PerVertex 
{
	vec4 gl_Position;
};

void main ()
{
	gl_Position = ubo.proj * ubo.view * ubo.model * vec4(inPosition, 1.0);

	Normal = inNormal;	
	FragPos = vec3(ubo.model * vec4(inPosition, 1.0));
}
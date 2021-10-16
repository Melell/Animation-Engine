#version 430 core

out vec4 fragColor;

in vec3 texCoords;

uniform samplerCube skyboxSampler;


void main()
{
	fragColor = /*vec4(1.0, 0.0, 0.0, 1.0);*/texture(skyboxSampler, texCoords);
}
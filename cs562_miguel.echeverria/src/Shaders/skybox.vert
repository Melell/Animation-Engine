#version 430 core

layout(location = 0) in vec3 attPosition;

uniform mat4 worldToView;
uniform mat4 perspectiveProj;

out vec3 texCoords;


void main()
{
	texCoords = attPosition;
	vec4 finalPos = perspectiveProj * worldToView * vec4(attPosition, 1.0);
	gl_Position = finalPos.xyww;
}
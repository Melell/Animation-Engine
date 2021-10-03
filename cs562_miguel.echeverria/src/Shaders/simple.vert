#version 430 core

layout(location = 0) in vec3 attPosition;

uniform mat4 modelToWorld;
uniform mat4 worldToView;
uniform mat4 perspectiveProj;

void main()
{
	gl_Position = perspectiveProj * worldToView * modelToWorld * vec4(attPosition, 1.0);
}
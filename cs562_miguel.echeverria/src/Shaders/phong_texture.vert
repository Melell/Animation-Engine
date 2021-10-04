#version 430 core

layout(location = 0) in vec3 attPosition;
layout(location = 1) in vec3 attNormal;
layout(location = 2) in vec2 attTexCoords;


uniform mat4 modelToWorld;		// Model to world transformation matrix
uniform mat4 worldToView;		// World to view/camera space transformation matrix
uniform mat4 perspectiveProj;	// Perspective projection transformation matrix
uniform mat3 normalViewMtx;		// Model to view space transformation specifically for normals


// The current vertex position and normal in view space, as well as the texture coordinates
out vec3 fragViewPos;
out vec3 viewNormal;
out vec2 texCoords;


void main()
{
	fragViewPos = vec3(worldToView * modelToWorld * vec4(attPosition, 1.0));
	viewNormal = normalViewMtx * attNormal;
	texCoords = attTexCoords;

	gl_Position = perspectiveProj * worldToView * modelToWorld * vec4(attPosition, 1.0);
}
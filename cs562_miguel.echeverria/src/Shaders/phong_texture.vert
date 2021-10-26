#version 430 core

#define MAX_JOINTS 128

layout(location = 0) in vec3 attPosition;
layout(location = 1) in vec3 attNormal;
layout(location = 2) in vec2 attTexCoords;
layout(location = 4) in vec4 attJoints;
layout(location = 5) in vec4 attJointWeights;


uniform mat4 modelToWorld;		// Model to world transformation matrix
uniform mat4 worldToView;		// World to view/camera space transformation matrix
uniform mat4 perspectiveProj;	// Perspective projection transformation matrix
uniform mat3 normalViewMtx;		// Model to view space transformation specifically for normals

uniform mat4 jointMatrices[MAX_JOINTS];
uniform bool useSkinning;

// The current vertex position and normal in view space, as well as the texture coordinates
out vec3 fragViewPos;
out vec3 viewNormal;
out vec2 texCoords;


void main()
{
	// Set out parameters
	fragViewPos = vec3(worldToView * modelToWorld * vec4(attPosition, 1.0));
	viewNormal = normalViewMtx * attNormal;
	texCoords = attTexCoords;

	// Compute the skinning matrix if necessary
	mat4 skinMatrix = mat4(1.0);
	if (useSkinning)
	{
		skinMatrix = attJointWeights.x * jointMatrices[int(attJoints.x)] +
					 attJointWeights.y * jointMatrices[int(attJoints.y)] +
					 attJointWeights.z * jointMatrices[int(attJoints.z)] +
					 attJointWeights.w * jointMatrices[int(attJoints.w)];
	}

	// Output position
	gl_Position = perspectiveProj * worldToView * modelToWorld * skinMatrix * vec4(attPosition, 1.0);
}
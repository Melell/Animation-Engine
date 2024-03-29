#version 430 core

#define MAX_JOINTS 128

layout(location = 0) in vec3 attPosition;
layout(location = 1) in vec3 attNormal;
layout(location = 2) in vec2 attTexCoords;
layout(location = 3) in vec3 attTangent;
layout(location = 4) in vec4 attJoints;
layout(location = 5) in vec4 attJointWeights;


struct Light
{
	vec3 m_direction;		// Direction of the light rays (only one used in vertex shader)
	vec3 m_ambient;
	vec3 m_diffuse;
	vec3 m_specular;
};


uniform Light light;
uniform vec3 camPosWorldSpace;

uniform mat4 modelToWorld;		// Model to world transformation matrix
uniform mat4 worldToView;		// World to view/camera space transformation matrix
uniform mat4 perspectiveProj;	// Perspective projection transformation matrix
uniform mat3 normalViewMtx;		// Model to view space transformation specifically for normals

uniform mat4 jointMatrices[MAX_JOINTS];
uniform bool useSkinning;

out vec3 fragPosTangentSpace;
out vec3 lightDirTangentSpace;
out vec3 camPosTangentSpace;
out vec2 texCoords;


void main()
{
	// Compute the tangent, normal and bitangent in view space (reorthogonalize to make sure we have an orthonormal basis)
	vec3 tangentViewSpace = normalize(normalViewMtx * attTangent);
	vec3 normalViewSpace = normalize(normalViewMtx * attNormal);
	tangentViewSpace = normalize(tangentViewSpace - dot(tangentViewSpace, normalViewSpace) * normalViewSpace);
	vec3 bitangentViewSpace = cross(normalViewSpace, tangentViewSpace);

	// Compute the tbn matrix (to go from view space to tangent space)
	mat3 viewToTangentMtx = transpose(mat3(tangentViewSpace, bitangentViewSpace, normalViewSpace));

	// Transform the necessary lighting vectors to tangent space
	fragPosTangentSpace = viewToTangentMtx * vec3(worldToView * modelToWorld * vec4(attPosition, 1.0));
	lightDirTangentSpace = viewToTangentMtx * mat3(worldToView) * light.m_direction;
	camPosTangentSpace = viewToTangentMtx * vec3(worldToView * vec4(camPosWorldSpace, 1.0));

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

	gl_Position = perspectiveProj * worldToView * modelToWorld * skinMatrix * vec4(attPosition, 1.0);
}
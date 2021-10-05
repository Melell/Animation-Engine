#version 430 core

out vec4 fragColor;				// Final output color


// Material properties of the object to be rendered
struct Material
{
	sampler2D m_diffuse;	// Diffuse, ambient, and specular texture are the same for now
	sampler2D m_normalMap;	// Normal map texture
	float m_shininess;		// How rough or smooth (shiny) the specular highlight is
};


// Directional light properties
struct Light
{
	vec3 m_direction;		// Direction of the light rays
	vec3 m_ambient;			// Color of the light's ambient component
	vec3 m_diffuse;			// Color of the light's diffuse component
	vec3 m_specular;		// Color of the light's specular component
};


uniform mat4 worldToView;

// Pass the material and light properties as uniforms
uniform Material mat;
uniform Light light;
uniform float normalColorScale;

// The necessary variables in tangent space for the lighting computations, as well as texture coordinates.
in vec3 fragPosTangentSpace;
in vec3 lightDirTangentSpace;
in vec3 camPosTangentSpace;
in vec2 texCoords;


void main()
{
	// Sample the normal map to get the tangent space normal and convert to range [-1, 1]
	vec3 normalTangentSpace = texture(mat.m_normalMap, texCoords).rgb;
	normalTangentSpace = normalize((normalTangentSpace * 2.0 - 1.0) * vec3(normalColorScale, normalColorScale, 1.0));

	vec3 finalLightDir = normalize(lightDirTangentSpace);

	// Store the material's diffuse color so that we don't have to resample for each computation
	vec3 matDiffuse = texture(mat.m_diffuse, texCoords).rgb;

	// Use the material's diffuse color to compute the ambient
	vec3 finalAmbient = light.m_ambient * matDiffuse;

	// Compute the diffuse component
	float diffuseIntensity = max(dot(normalTangentSpace, -finalLightDir), 0.0);
	vec3 finalDiffuse = diffuseIntensity * light.m_diffuse * matDiffuse;

	// Compute the specular component
	vec3 viewVec = normalize(camPosTangentSpace - fragPosTangentSpace);
	vec3 reflectionVec = reflect(finalLightDir, normalTangentSpace);
	float specIntensity = pow(max(dot(reflectionVec, viewVec), 0.0), mat.m_shininess);
	vec3 finalSpecular = specIntensity * light.m_specular * matDiffuse;

	// Add all the components to get the final color
	fragColor = vec4(finalAmbient + finalDiffuse + finalSpecular, 1.0);
}
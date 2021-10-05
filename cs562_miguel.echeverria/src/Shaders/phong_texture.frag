#version 430 core

out vec4 fragColor;				// Final output color


// Material properties of the object to be rendered
struct Material
{
	sampler2D m_diffuse;	// Diffuse, ambient and specular texture are the same for now
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

// The current fragment position and normal in view space, as well as its texture coordinates
in vec3 fragViewPos;
in vec3 viewNormal;
in vec2 texCoords;


void main()
{
	vec3 lightDirViewSpace = normalize(vec3(worldToView * vec4(light.m_direction, 0.0)));
	vec3 normalizedNormal = normalize(viewNormal);

	// Use the material's diffuse color to compute the ambient
	vec3 finalAmbient = light.m_ambient * texture(mat.m_diffuse, texCoords).rgb;

	// Compute the diffuse component
	float diffuseIntensity = max(dot(normalizedNormal, -lightDirViewSpace), 0.0);
	vec3 finalDiffuse = diffuseIntensity * light.m_diffuse * texture(mat.m_diffuse, texCoords).rgb;

	// Compute the specular component
	vec3 viewVec = normalize(-fragViewPos);
	vec3 reflectionVec = reflect(lightDirViewSpace, normalizedNormal);
	float specIntensity = pow(max(dot(reflectionVec, viewVec), 0.0), mat.m_shininess);
	vec3 finalSpecular = specIntensity * light.m_specular * texture(mat.m_diffuse, texCoords).rgb;

	// Add all the components to get the final color
	fragColor = vec4(finalAmbient + finalDiffuse + finalSpecular, 1.0);
}
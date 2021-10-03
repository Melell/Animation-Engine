#version 430 core

out vec4 fragColor;				// Final output color


// Material properties of the object to be rendered
struct Material
{
	vec3 m_diffuse;				// Diffuse and ambient color are the same
	vec3 m_specular;			// Specular color that can be reflected
	float m_shininess;			// How rough or smooth (shiny) the specular highlight is
};


// Directional light properties
struct Light
{
	vec3 m_direction;			// Direction of the light rays
	float m_ambientIntensity;	// Strength of the ambient component
	float m_diffuseIntensity;	// Strength of the diffuse component
	float m_specularIntensity;	// Strength of the specular component
};


// Pass the material and light properties as uniforms
uniform Material mat;
uniform Light light;

// The current fragment position and normal in view space
in vec3 fragViewPos;
in vec3 viewNormal;


void main()
{
	
}
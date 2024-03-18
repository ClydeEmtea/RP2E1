#version 330 core

// Outputs colors in RGBA
out vec4 FragColor;


// Imports the color from the Vertex Shader
in vec3 color;
// Imports the texture coordinates from the Vertex Shader
in vec2 texCoord;
// Imports the normal from the Vertex Shader
in vec3 Normal;
// Imports the current position from the Vertex Shader
in vec3 crntPos;

// Gets the Texture Units from the main function
uniform sampler2D tex0;
uniform sampler2D tex1;
// Gets the color of the light from the main function
uniform vec4 lightColor;
// Gets the position of the light from the main function
uniform vec3 lightPos;
// Gets the position of the camera from the main function
uniform vec3 camPos;

uniform float near;


vec4 pointLight()
{	
	// used in two variables so I calculate it here to not have to do it twice
	vec3 lightVec = lightPos - crntPos;

	// intensity of light with respect to distance
	float dist = length(lightVec);
	float a = 0.001;
	float b = 0.5;
	float inten = 1.0f / (a * dist * dist + b * dist + 1.0f);

	// ambient lighting 
	float ambient = 0.05f;

	// diffuse lighting 
	vec3 normal = normalize(Normal);
	vec3 lightDirection = normalize(lightVec);
	float diffuse = max(dot(normal, lightDirection), 0.0f);
	diffuse = max(diffuse, 0.2f); // Ensure a minimum value for diffuse

	return (texture(tex0, texCoord) * (diffuse * inten + ambient)) * lightColor;
}



float far = 100.0f;
float linearizeDepth(float depth)
{
	float z = depth * 2.0f - 1.0f;  
	return (2.0f * near * far) / (far + near - z * (far - near));	
}

float logisticsDepth(float depth, float steepness = 0.5f, float offset = 5.0f)
{
	float zVal = linearizeDepth(depth);
	return (1 / (1 + exp(-steepness * (zVal - offset))));
}

void main()
{
	// outputs final color
	// FragColor = pointLight();
	float depth = logisticsDepth(gl_FragCoord.z);
	FragColor = pointLight() * 1.3f * (1.0f - depth) + vec4(depth * vec3(0.01f, 0.01f, 0.02f), 1.0f);
}
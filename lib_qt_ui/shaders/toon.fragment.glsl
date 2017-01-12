#version 330

// uniform inputs
struct Light
{
    vec4 position;  // Light position in eye coords.
    vec3 direction;
    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
    float constantAttenuation;
    float linearAttenuation;
    float quadraticAttenuation;
};

uniform Light lights[1];

struct Material
{
    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
    float shininess;
};

uniform Material material;

in vec3 position;
in vec3 normal;
in float altitude;

out vec4 fragColor;

const int levels = 4;
const float scaleFactor = 1.0 / levels;

vec3 toonShade(const in vec3 pos, const in vec3 norm)
{
    vec3 matAmbient, matDiffuse;
    if (altitude > -100.)
    {
            matAmbient = material.ambient.xyz;
            matDiffuse = material.diffuse.xyz;
    }
    else
    {
            matAmbient = vec3(0.2, 0.2, 0.6);
            matDiffuse = vec3(0.2, 0.2, 0.6);
    }
	
    // If the triangle is wound the wrong way, invert the normal so
    // that we get the desired lighting effect
    vec3 n = norm;
    if ( !gl_FrontFacing )
        n = -n;

    // Calculate the vector from the light to the fragment
    vec3 s = normalize(vec3(lights[0].position) - pos);

    // Calculate the diffuse component
    float cosine = dot(s, n);
    float diffuse = floor(cosine * levels) * scaleFactor;

    // Combine the ambient and diffuse contributions. No specular!
    return lights[0].ambient.xyz * (matAmbient + matDiffuse * diffuse);
}

void main()
{
    fragColor = vec4(toonShade(position, normal), 1.0);
}

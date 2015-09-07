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

uniform Light lights[8];
uniform int lightCount;

struct Material
{
	vec4 ambient;
	vec4 diffuse;
	vec4 specular;
	float shininess;
	float opacity;
};

uniform Material material;

// glsl implementation of refract function from Cg stdlib
vec3 refract(vec3 i, vec3 n, float eta)
{
    float cosi = dot(-i, n);
    float cost2 = 1.0 - eta * eta * (1.0 - cosi*cosi);
    vec3 t = eta*i + ((eta*cosi - sqrt(abs(cost2))) * n);
    return t * vec3(cost2 > 0.0);
}

// fresnel approximation
float fast_fresnel(vec3 I, vec3 N, vec3 fresnelValues)
{
    float power = fresnelValues.x;
    float scale = fresnelValues.y;
    float bias = fresnelValues.z;

    return bias + pow(1.0 - dot(I, N), power) * scale;
}

// inputs
in vec3 normal;
in vec3 incident;

// uniform inputs
//uniform vec3 etaValues;
//uniform vec3 fresnelValues;

// outputs
out vec4 fragColor;

uniform samplerCube cubeMapTex;

void main()
{
	vec3 etaValues = vec3(1.1, 1.08, 1.06);
	vec3 fresnelValues = vec3(2.0, 2.0, 0.1);
	//
    // normalize incoming vectors
    vec3 normalVec = normalize(normal);
    vec3 incidentVec = normalize(incident);

    vec3 refractColor;

    // calculate refract color for each color channel
    refractColor.r = texture(cubeMapTex, refract(incidentVec, normalVec, etaValues.x)).r;
    refractColor.g = texture(cubeMapTex, refract(incidentVec, normalVec, etaValues.y)).g;
    refractColor.b = texture(cubeMapTex, refract(incidentVec, normalVec, etaValues.z)).b;

    // fetch reflection from environment map
    vec3 reflectColor = texture(cubeMapTex, reflect(incidentVec, normalVec)).rgb;

    vec3 fresnelTerm = vec3(fast_fresnel(-incidentVec, normalVec, fresnelValues));
    fragColor = vec4(mix(refractColor, reflectColor, fresnelTerm), material.opacity);
}

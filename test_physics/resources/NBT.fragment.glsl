#version 330

uniform sampler2D textureDiffuse0; // texture des couleurs
uniform sampler2D textureNormals0; // texture des normales


// uniform inputs
struct Light
{
    bool enabled;
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

in vec3 texCoord;
in vec3 normal;
in vec3 position;

in vec4 lightDir;
in vec4 eyeVec;


out vec4 fragColor;

// http://www.thetenthplanet.de/archives/1180
mat3 cotangent_frame(vec3 N, vec3 p, vec2 uv)
{
    // récupère les vecteurs du triangle composant le pixel
    vec3 dp1 = dFdx(p);
    vec3 dp2 = dFdy(p);
    vec2 duv1 = dFdx(uv);
    vec2 duv2 = dFdy(uv);

    // résout le système linéaire
    vec3 dp2perp = cross(dp2, N);
    vec3 dp1perp = cross(N, dp1);
    vec3 T = dp2perp * duv1.x + dp1perp * duv2.x;
    vec3 B = dp2perp * duv1.y + dp1perp * duv2.y;

    // construit une trame invariante à l'échelle 
    float invmax = inversesqrt(max( dot(T,T), dot(B,B)));
    return mat3(T * invmax, B * invmax, N);
}

vec3 perturb_normal(vec3 N, vec3 V, vec2 texcoord)
{
    // N, la normale interpolée et
    // V, le vecteur vue (vertex dirigé vers l'œil)
    vec3 map = texture(textureNormals0, texcoord).xyz;
    map = map * 255./127. - 128./127.;
    mat3 TBN = cotangent_frame(N, -V, texcoord);
    return normalize(TBN * map);
}

void phongModel(const in int lightIndex, vec3 pos, vec3 norm, out vec3 ambientAndDiff, out vec3 spec)
{
    vec3 s = normalize(vec3(lights[lightIndex].position) - pos);
    vec3 v = normalize(-pos.xyz);

    vec3 r = reflect(-s, norm);

    // Calculate the ambient contribution
    vec3 ambient = vec3(lights[lightIndex].ambient * material.ambient);

    // Calculate the diffuse contribution
    float sDotN = max(dot(s, norm), 0.0);
    vec3 diffuse = vec3(lights[lightIndex].diffuse * material.diffuse) * sDotN;

    // Sum the ambient and diffuse contributions
    ambientAndDiff = ambient + diffuse;

    // Calculate the specular highlight component
    spec = vec3(0.0);
    if (sDotN > 0.0)
    {
        spec = vec3(lights[lightIndex].specular * material.specular) * pow(max(dot(r, v), 0.0), material.shininess);
    }
}

void main()
{
    vec2 uv = texCoord.xy;

    vec3 N = normalize(normal.xyz);
    vec3 L = normalize(lightDir.xyz);
    vec3 V = normalize(eyeVec.xyz);
    vec3 PN = perturb_normal(N, V, uv);

    vec4 textureColor = texture(textureDiffuse0, vec2(texCoord));

    vec4 color = vec4(0.0);
    for (int i = 0; i < lightCount; ++i)
    {
        if (lights[i].enabled)
        {
            // Calculate the lighting model, keeping the specular component separate
            vec3 ambientAndDiff, spec;

            phongModel(i, position, PN, ambientAndDiff, spec);

            color += (vec4(ambientAndDiff, 1.0) * textureColor + vec4(spec, 1.0)); //float(lightCount);
        }
    }

    fragColor = vec4(color.xyz, material.opacity);
}

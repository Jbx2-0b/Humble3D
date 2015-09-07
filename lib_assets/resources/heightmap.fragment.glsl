#version 330

in vec3 vsTexCoord;
in vec3 vsNormal;
in vec3 vsLocalNormal;
in vec3 vsPosition;
in vec3 vsLocalPosition;

uniform sampler2D textureDiffuse0;
uniform sampler2D textureDiffuse1;
uniform sampler2D textureDiffuse2;
uniform sampler2D textureDiffuse3;


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

uniform struct Fog
{
    vec4 color;
    float density;
};

uniform Fog fog;


// outputs
out vec4 fragColor;


void threshold(in float dValue, in float s1, in float s2, in float s3, in float s4, out vec4 color)
{
    vec4 t1Color = texture2D(textureDiffuse0, vec2(vsTexCoord));
    vec4 t2Color = texture2D(textureDiffuse1, vec2(vsTexCoord));
    vec4 t3Color = texture2D(textureDiffuse2, vec2(vsTexCoord));
    vec4 t4Color = texture2D(textureDiffuse3, vec2(vsTexCoord));

    if (dValue <= s1)
    {
        color = t1Color;
    }
    else if (dValue < s2)
    {
        float f = smoothstep(s1, s2, dValue);
        color = mix(t1Color, t2Color, f);
    }
    else if (dValue < s3)
    {
        float f = smoothstep(s2, s3, dValue);
        color = mix(t2Color, t3Color, f);
    }
    else if (dValue < s4)
    {
        float f = smoothstep(s3, s4, dValue);
        color = mix(t3Color, t4Color, f);
    }
    else
    {
        color = t4Color;
    }
}


void phongModel(const in int lightIndex, vec3 pos, vec3 norm, out vec3 ambientAndDiff, out vec3 spec)
{
    // Some useful vectors
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
    vec4 col;

    if (vsLocalPosition.y > -100.)
    {
        threshold(vsLocalNormal.y, 0.25, 0.90, 0.98, 1., col);
    }
    else
    {
        col = vec4(0.2, 0.2, 0.6, 1.0);
    }

    vec4 color = vec4(0.0);
    for (int i = 0; i < lightCount; ++i)
    {
        // Calculate the lighting model, keeping the specular component separate
        vec3 ambientAndDiff, spec;

        phongModel(i, vsPosition, vsNormal, ambientAndDiff, spec);

        color += (vec4(ambientAndDiff, 1.0) * col + vec4(spec, 1.0)); // / float(lightCount);
    }

    const float LOG2 = 1.442695;
    float z = abs(vsPosition.z);
    float fogFactor = exp2(-fog.density * fog.density * z * z * LOG2);

    fogFactor = clamp(fogFactor, 0.0, 1.0);

    vec4 finalColor = mix(fog.color, color, fogFactor);

    fragColor = vec4(color.xyz, material.opacity);
}


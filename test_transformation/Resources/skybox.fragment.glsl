#version 330

in vec3 vsLocalPosition;

uniform samplerCube cubeMapTex;

layout(location = 0) out vec4 fragColor;

void main()
{
    vec4 col = texture(cubeMapTex, vsLocalPosition);
    fragColor = col;
}


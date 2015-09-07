varying vec3 vsLocalPosition;

uniform samplerCube cubeMapTex;

void main()
{
    vec4 col = textureCube(cubeMapTex, vsLocalPosition);
    gl_FragColor = col;
}


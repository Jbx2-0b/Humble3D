#version 330

uniform sampler2D textureDiffuse0;

in vec3 texCoord;

void main (void)
{
    float blurValue = 0.0015;

    vec4 color = texture2D(textureDiffuse0, vec2(texCoord.s + blurValue, texCoord.t + blurValue));
    color += texture2D(textureDiffuse0, vec2(texCoord.s - blurValue, texCoord.t + blurValue));
    color += texture2D(textureDiffuse0, vec2(texCoord.s + blurValue, texCoord.t - blurValue));
    color += texture2D(textureDiffuse0, vec2(texCoord.s - blurValue, texCoord.t - blurValue));

    gl_FragColor = color / 4.0;
}

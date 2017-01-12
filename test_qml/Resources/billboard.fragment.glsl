#version 330

uniform sampler2D textureDiffuse0;

in vec2 gsTexCoord;
out vec4 fragColor;

void main()
{
    fragColor = texture2D(textureDiffuse0, gsTexCoord);

    if (fragColor.a == 0.)
    {
        discard;
    }
} 

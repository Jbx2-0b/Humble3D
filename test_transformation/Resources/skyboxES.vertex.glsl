// inputs
attribute vec3 vertexPosition;

uniform mat4 modelViewProjectionMatrix;

varying vec3 vsLocalPosition;

void main()
{
    vsLocalPosition = vertexPosition;
    gl_Position = modelViewProjectionMatrix * vec4(vertexPosition, 1.0);
}

#ifdef GL_ES
// Set default precision to medium
precision mediump int;
precision mediump float;
#endif

// inputs
attribute vec3 vertexPosition;
attribute vec3 vertexNormal;

varying vec3 position;
varying vec3 normal;
varying float altitude;

uniform mat4 modelViewMatrix;
uniform mat3 normalMatrix;
uniform mat4 projectionMatrix;
uniform mat4 modelViewProjectionMatrix;

void main()
{
    altitude = vertexPosition.y;
    normal = normalize(normalMatrix * vertexNormal);
    position = vec3(modelViewMatrix * vec4(vertexPosition, 1.0));

    gl_Position = modelViewProjectionMatrix * vec4(vertexPosition, 1.0);
}

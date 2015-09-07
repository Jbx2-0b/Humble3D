#ifdef GL_ES
// Set default precision to medium
precision mediump int;
precision mediump float;
#endif

// inputs
attribute vec3 vertexPosition;
attribute vec3 vertexNormal;
attribute vec3 vertexTexCoord;


// uniform inputs
uniform mat4 modelViewMatrix;
uniform mat3 normalMatrix;
uniform mat4 projectionMatrix;
uniform mat4 modelViewProjectionMatrix;


// outputs
varying vec3 texCoord;
varying vec3 normal;
varying vec3 position;


void main()
{
    texCoord = vertexTexCoord;

    // Transform normal and tangent to eye space
    normal = normalize(normalMatrix * vertexNormal);
    //vec3 tang = normalize(normalMatrix * vec3(vertexTangent));

    position = vec3(modelViewMatrix * vec4(vertexPosition, 1.0));

    // Calculate vertex position in screen space
    gl_Position = modelViewProjectionMatrix * vec4(vertexPosition, 1.0);
}

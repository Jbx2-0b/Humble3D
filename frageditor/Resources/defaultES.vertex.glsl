uniform mat4 modelViewProjectionMatrix;

attribute vec3 vertexPosition;

void main()
{    
    gl_Position = modelViewProjectionMatrix * vec4(vertexPosition, 1.0);
}



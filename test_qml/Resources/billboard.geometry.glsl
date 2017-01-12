#version 330
#extension GL_EXT_geometry_shader4 : enable

layout (points) in;
layout (triangle_strip, max_vertices = 4) out;

uniform mat4 modelViewMatrix;
uniform mat4 projectionMatrix;
uniform float halfSize;

in vec4 vsPosition[];

out vec2 gsTexCoord;

void main()
{
    gl_Position = projectionMatrix * ((vsPosition[0] + vec4(-halfSize, +halfSize, 0., 1.)) + vec4(modelViewMatrix[3].xyz, 0));
    gsTexCoord = vec2(0.0, 0.0);
    EmitVertex();

    gl_Position = projectionMatrix * ((vsPosition[0] + vec4(-halfSize, -halfSize, 0., 1.)) + vec4(modelViewMatrix[3].xyz, 0));
    gsTexCoord = vec2(0.0, 1.0);
    EmitVertex();

    gl_Position = projectionMatrix * ((vsPosition[0] + vec4(+halfSize, +halfSize, 0., 1.)) + vec4(modelViewMatrix[3].xyz, 0));
    gsTexCoord = vec2(1.0, 0.0);
    EmitVertex();

	gl_Position = projectionMatrix * ((vsPosition[0] + vec4(+halfSize, -halfSize, 0., 1.)) + vec4(modelViewMatrix[3].xyz, 0));
    gsTexCoord = vec2(1.0, 1.0);
    EmitVertex();

    EndPrimitive();
} 
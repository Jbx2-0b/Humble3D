#version 330
#extension GL_EXT_geometry_shader4 : enable

layout (triangles) in;
layout (triangle_strip,  max_vertices = 3) out;


uniform mat4 modelViewProjectionMatrix;


in vec3 vsTexCoord[];
in vec3 vsNormal[];
in vec3 vsLocalNormal[];
in vec3 vsPosition[];
in vec3 vsLocalPosition[];

out vec3 gsTexCoord;
out vec3 gsNormal;
out vec3 gsLocalNormal;
out vec3 gsPosition;
out vec3 gsLocalPosition;
flat out int gsIsGrass;


void main (void)
{
    for (int i = 0; i < gl_VerticesIn; ++i)
    {
        gl_Position = modelViewProjectionMatrix * gl_PositionIn[i];
        gsTexCoord = vsTexCoord[i];
        gsNormal = vsNormal[i];
        gsLocalNormal = vsLocalNormal[i];
        gsPosition = vsPosition[i];
        gsLocalPosition = vsLocalPosition[i];
        gsIsGrass = 0;
        EmitVertex ();
    }
    EndPrimitive ();

    /*
    for (int c = 1; c < 11; ++c)
    {
        for (int i = 0; i < gl_VerticesIn; ++i)
        {
            vec4 tPos = gl_PositionIn[i];
            tPos.y += c;
            gl_Position = modelViewProjectionMatrix * tPos;
            gsTexCoord = vsTexCoord[i];
            gsNormal = vsNormal[i];
            gsLocalNormal = vsLocalNormal[i];
            gsPosition = vsPosition[i];
            gsIsGrass = 1;
            EmitVertex ();
        }
        EndPrimitive ();
    }*/
}

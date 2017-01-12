#version 330

// inputs
layout (location = 0) in vec3 vertexPosition;
layout (location = 1) in vec3 vertexNormal;
layout (location = 2) in vec3 vertexTexCoord;
layout (location = 3) in vec3 vertexTangent;
layout (location = 4) in float vertexBoneIDs;
layout (location = 5) in float vertexWeights;


const int MAX_BONES = 100;

// uniform inputs
uniform mat4 modelViewProjectionMatrix;
uniform mat4 modelViewMatrix;
uniform mat3 normalMatrix;
uniform mat4 projectionMatrix;
uniform mat4 bones[MAX_BONES];


uniform bool hasSkeleton;

   
// outputs
out vec3 texCoord;
out vec3 normal;
out vec3 position;


void main()
{
    texCoord = vertexTexCoord;

    if (hasSkeleton)
    {
        mat4 boneTransform = bones[int(vertexBoneIDs)] * vertexWeights;
        //boneTransform += bones[int(vertexBoneIDs[1])] * vertexWeights[1];
        //boneTransform += bones[int(vertexBoneIDs[2])] * vertexWeights[2];
        //boneTransform += bones[int(vertexBoneIDs[3])] * vertexWeights[3];

        // Transform normal and tangent to eye space
        normal = normalize(normalMatrix * mat3(boneTransform) * vertexNormal);
        //vec3 tang = normalize(normalMatrix * mat3(boneTransform) * vec3(vertexTangent));

        position = vec3(modelViewMatrix * boneTransform * vec4(vertexPosition, 1.0));


        // Calculate vertex position in screen space
        gl_Position = modelViewProjectionMatrix * boneTransform * vec4(vertexPosition, 1.0);
    }
    else
    {
        // Transform normal and tangent to eye space
        normal = normalize(normalMatrix * vertexNormal);
        //vec3 tang = normalize(normalMatrix * vec3(vertexTangent));

        position = vec3(modelViewMatrix * vec4(vertexPosition, 1.0));

        // Calculate vertex position in screen space
        gl_Position = modelViewProjectionMatrix * vec4(vertexPosition, 1.0);
    }
}

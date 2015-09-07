#version 330

// uniform inputs
uniform vec4 eyePosition;
uniform mat3 normalMatrix;
uniform mat4 viewInverseMatrix;
uniform mat4 modelViewMatrix; 
uniform mat4 modelViewProjectionMatrix;

// inputs
layout (location = 0) in vec3 vertexPosition;
layout (location = 1) in vec3 vertexNormal;

// outputs
out vec3 normal;
out vec3 incident;
//out vec3 position;

void main()
{
	const vec4 displace = vec4(5.0, 0.0, 0.02, 0.0);
	
    // deformation
    float deformation = sin(vertexPosition.y * displace.x + displace.y) * displace.z;
	//vec4 position = (vec4(vertexPosition, 1.0) + vec4(vertexNormal, 1.0) * deformation);
	//position = vec3(modelViewMatrix * vec4(vertexPosition, 1.0));
    // transform normal to world space
    normal = normalMatrix * vertexNormal; // mul by modelIT

	// transform position to world space
    vec4 worldPos = modelViewMatrix * vec4(vertexPosition, 1.0); // mul by model

    // transform eye position to world space
    vec4 worldEyePos = viewInverseMatrix * eyePosition; 
    
    // calculate incident vector
    incident = worldPos.xyz - eyePosition.xyz;

    // output position
    gl_Position = modelViewProjectionMatrix * vec4(vertexPosition, 1.0);
}
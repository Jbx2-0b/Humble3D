struct Material
{
	vec4 ambient;
	vec4 diffuse;
	vec4 specular;
	float shininess;
	float opacity;
};

uniform Material material;

void main()
{
    // Set fragment color
	gl_FragColor = vec4(material.ambient); 
}

#version 410

out vec4 gl_FragColor;

in vec3 vertexNormalOut;
in vec3 cameraDirectionOut;
in vec2 vertexTextureCoordsOut;


uniform vec3 lightDirection=vec3(2.0f,-1.0f,2.0f);


uniform vec4 specularMaterialColour=vec4(1.0f,1.0f,1.0f,1.0f);
uniform float specularPower=10.0f;
uniform vec4 specularLightColour=vec4(1.0f,1.0f,1.0f,1.0f);

uniform sampler2D diffuse;

void main()
{
	vec3 lightDir=normalize(-lightDirection);
	vec3 halfWayVec = normalize(cameraDirectionOut + lightDir);
	float specularTerm = pow(dot(vertexNormalOut, halfWayVec), specularPower);

	gl_FragColor=texture2D(diffuse, vertexTextureCoordsOut)+(specularMaterialColour*specularLightColour*specularTerm);
}
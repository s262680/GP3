#version 410

out vec4 gl_FragColor;

in vec3 vertexNormalOut;
in vec3 cameraDirectionOut;
in vec2 vertexTextureCoordsOut;
in vec3 camPos;
in vec3 worldPos;
uniform vec3 lightDirection=vec3(2.0f,-1.0f,2.0f);



uniform vec4 specularMaterialColour=vec4(1.0f,1.0f,1.0f,1.0f);
uniform float specularPower=10.0f;
uniform vec4 specularLightColour=vec4(1.0f,1.0f,1.0f,1.0f);

uniform sampler2D diffuse;

void main()
{

float actualDis=0;
	float startFogDis=1;
	float maxReachDis=15;


	float distance = length( worldPos - camPos);

	//actualDis = (maxReachDis - distance)/(maxReachDis - startFogDis);	
	actualDis = 1.0 /exp( (distance * 0.1)* (distance * 0.1));
    actualDis = clamp( actualDis, 0.0, 1.0 );

	vec3 lightDir=normalize(-lightDirection);
	vec3 halfWayVec = normalize(cameraDirectionOut + lightDir);
	float specularTerm = pow(dot(vertexNormalOut, halfWayVec), specularPower);

	//gl_FragColor = (ambientMaterialColour*ambientLightColour) + (diffuseMaterialColour*diffuseLightColour*diffuseTerm) + (specularMaterialColour*specularLightColour*specularTerm);
	gl_FragColor=mix(vec4(0.6f, 0.6f, 0.6f, 1.0f),texture2D(diffuse, vertexTextureCoordsOut)+(specularMaterialColour*specularLightColour*specularTerm),actualDis);
}
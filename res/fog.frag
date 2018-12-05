#version 450

out vec4 gl_FragColor;
in vec2 texCoord0;
in vec3 fragPos;
in vec3 camPos;
uniform sampler2D diffuse;

void main()
{
	float fogDisDensity=0;
	float startFogDis=1;
	float maxReachDis=15;
	vec4 fogColor=vec4(0.6f, 0.6f, 0.6f, 1.0f);
	float density=0.1f;

	float distance = length(fragPos - camPos);

	//linear fog
	//fogDisDensity = (maxReachDis - distance)/(maxReachDis - startFogDis);
	
	//exponential fog
	fogDisDensity = 1.0 /exp(pow((distance * density),2.0f));

    fogDisDensity = clamp(fogDisDensity, 0.0, 1.0 );
 
	//gl_FragColor=mix(fogColor,texture2D(diffuse, texCoord0),  min(3.0f / distance, 1.0f));
	gl_FragColor=mix(fogColor,texture2D(diffuse, texCoord0),  fogDisDensity);
}

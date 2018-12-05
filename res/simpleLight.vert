#version 410

layout(location=0) in vec3 position;
layout(location=1) in vec2 texCoord;
layout(location=2) in vec3 normal;



out vec2 vertexTextureCoordsOut;
out vec3 vertexNormalOut;
out vec3 cameraDirectionOut;



uniform mat4 MVP;
uniform mat4 Model;
uniform vec3 cameraPos;


void main()
{
	vertexNormalOut = normalize(Model*vec4(normal, 0.0f)).xyz;
	vec3 worldPos = (Model*vec4(position, 1.0)).xyz;
	cameraDirectionOut = normalize(cameraPos - worldPos);

	gl_Position = MVP * vec4(position, 1.0);
	vertexTextureCoordsOut=texCoord;

}
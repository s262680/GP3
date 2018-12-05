#version 410


layout(location=0) in vec3 position;
layout(location=1) in vec2 texCoord;
layout(location=2) in vec3 normal;





out vec3 world_pos;
out vec3 world_normal;
out vec2 texcoord;
out vec4 viewSpace;
out vec3 camPos;


uniform mat4 MVP;
uniform mat4 Model;
uniform mat4 view_space;
uniform vec3 cameraPos=vec3(-20.0f,10.0f,-20.0f);



void main()
{
world_pos = (Model*vec4(position, 1.0)).xyz;
world_normal=normalize(mat3(Model)*normal);
texcoord=texCoord;
viewSpace=view_space*Model*vec4(position,1);

gl_Position = MVP * vec4(position, 1.0);
camPos=cameraPos;
}
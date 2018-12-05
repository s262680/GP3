#version 410

out vec4 gl_FragColor;
 
uniform vec3 light_position=vec3(-20.0f,10.0f,-20.0f);
uniform vec3 eye_position=vec3(0.0f, 0.0f, 0.0f);
 
uniform sampler2D diffuse;
 

//uniform int fogSelector;

//uniform int depthFog;
 

const vec3 DiffuseLight = vec3(0.15, 0.05, 0.0);
const vec3 RimColor = vec3(0.2, 0.2, 0.2);
 

in vec3 world_pos;
in vec3 world_normal;
in vec4 viewSpace;
in vec2 texcoord;
in vec3 camPos;
in vec3 worldPos;

const vec3 fogColor = vec3(0.5, 0.5,0.5);
const float FogDensity = 0.05;
 
void main(){
 
vec3 tex1 = texture(diffuse, texcoord).rgb;
 

vec3 L = normalize( light_position - world_pos);
vec3 V = normalize( eye_position - world_pos);
 

vec3 diffuse0 = DiffuseLight * max(0, dot(L,world_normal));
 

float rim = 1 - max(dot(V, world_normal), 0.0);
rim = smoothstep(0.6, 1.0, rim);
vec3 finalRim = RimColor * vec3(rim, rim, rim);

vec3 lightColor = finalRim + diffuse0 + tex1;
 
vec3 finalColor = vec3(0, 0, 0);
 

float dist = 0;
float fogFactor = 0;
 


  dist = length( world_pos - camPos);

 fogFactor = (15 - dist)/(15 - 1);
   fogFactor = clamp( fogFactor, 0.0, 1.0 );
 
  
   finalColor = mix(fogColor, lightColor, fogFactor);

gl_FragColor = vec4(finalColor, 1);
 
}
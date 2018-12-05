#version 450

out vec4 gl_FragColor;
in vec2 texCoord0;

uniform sampler2D diffuse;

void main()
{
	gl_FragColor = texture2D(diffuse, texCoord0);

}

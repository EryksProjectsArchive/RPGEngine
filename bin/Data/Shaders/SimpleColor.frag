#version 150

uniform sampler2D texture;

out vec4 fragmentColor;

in vec4 vColor;

void main()
{
	fragmentColor = vColor;
}
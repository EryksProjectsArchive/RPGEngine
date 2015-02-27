#version 150

uniform sampler2D texture;

out vec4 fragmentColor;

in vec4 vColor;
in vec2 vUV;

void main()
{
	fragmentColor = vColor * texture2D(texture, vUV);
}
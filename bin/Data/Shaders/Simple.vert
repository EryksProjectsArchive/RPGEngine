#version 150

uniform mat4 mvp;

in vec2 vertexPosition;
in vec4 vertexColor;
in vec2 vertexUV;

out vec4 vColor;
out vec2 vUV;

void main()
{
	gl_Position = mvp * vec4(vertexPosition, 0, 1);
	vColor = vertexColor;
	vUV = vertexUV;
}
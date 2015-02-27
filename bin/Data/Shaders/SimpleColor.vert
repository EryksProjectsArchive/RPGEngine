#version 150

uniform mat4 mvp;

in vec2 vertexPosition;
in vec4 vertexColor;

out vec4 vColor;

void main()
{
	gl_Position = mvp * vec4(vertexPosition, 0, 1);
	vColor = vertexColor;
}
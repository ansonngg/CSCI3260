#version 430

in layout(location = 0) vec3 position;

uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

out vec3 texCoords;

void main()
{
	texCoords = position;
	gl_Position = projectionMatrix * viewMatrix * vec4(position, 1.0);
}
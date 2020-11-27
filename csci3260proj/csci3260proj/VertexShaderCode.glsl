#version 430

in layout(location = 0) vec3 position;
in layout(location = 1) vec2 vertexUV;
in layout(location = 2) vec3 vertexNormal;

uniform mat4 modelMatrix;
uniform mat4 eyeWorldMatrix;

out vec2 UV;
out vec3 eyeWorldNormal;

void main()
{
	vec4 newPosition = modelMatrix * vec4(position, 1.0);
	gl_Position = eyeWorldMatrix * newPosition;
	UV = vertexUV;
	eyeWorldNormal = (modelMatrix * vec4(vertexNormal, 0)).xyz;
}
#version 430

struct Material {
	sampler2D myTexture;
};

in vec2 UV;

uniform Material material;

out vec4 Color;

void main()
{
	Color = texture(material.myTexture, UV);
}
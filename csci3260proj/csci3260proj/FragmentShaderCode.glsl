#version 430

struct Material {
	sampler2D myTexture;
	sampler2D myNormalTexture;
};

in vec2 UV;
in vec3 eyeWorldNormal;

uniform Material material;
uniform bool normalMappingFlag;

out vec4 Color;

void main()
{
	vec3 normal = normalize(eyeWorldNormal);
	if(normalMappingFlag) {
		normal = texture(material.myNormalTexture, UV).rgb;
		normal = normalize(normal * 2.0 - 1.0);
	}
	Color = texture(material.myTexture, UV);
}
#version 330 core
out vec4 FragColor;

in vec2 TexCoord;

// texture samplers
uniform sampler2D diffuse;
uniform sampler2D specular;

void main()
{
	// linearly interpolate between both textures (80% container, 20% awesomeface)
	FragColor = mix(texture(diffuse, TexCoord), texture(specular, TexCoord), 0.2);
}
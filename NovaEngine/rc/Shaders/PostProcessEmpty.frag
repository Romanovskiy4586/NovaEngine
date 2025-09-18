#version 460 core

in vec2 textureCoordinates;

out vec4 FragColor;

uniform sampler2D image;

void main()
{
	FragColor = texture(image, textureCoordinates);
}
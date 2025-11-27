#version 460 core

out vec4 FragColor;

in vec2 textureCoordinates;

uniform sampler2D albedo;

void main()
{
    FragColor = texture(albedo, textureCoordinates);
}
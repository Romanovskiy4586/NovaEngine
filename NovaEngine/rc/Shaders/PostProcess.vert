#version 460 core

in vec2 aPosition;
in vec2 aTextureCoordinates;

out vec2 textureCoordinates;

void main()
{
    gl_Position = vec4(aPosition, 0.0, 1.0);
    textureCoordinates = aTextureCoordinates;
}
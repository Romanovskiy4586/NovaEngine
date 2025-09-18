#version 460 core

in vec3 aPosition;
in vec2 aTextureCoordinates;

out vec2 textureCoordinates;

uniform mat4 pvm;

void main()
{
    gl_Position = pvm * vec4(aPosition, 1.0);
    textureCoordinates = aTextureCoordinates;
}
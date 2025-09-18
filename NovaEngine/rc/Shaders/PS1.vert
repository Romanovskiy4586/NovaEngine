#version 460 core

in vec3 aPosition;
in vec2 aTextureCoordinates;

out vec2 textureCoordinates;

uniform mat4 pvm;

void main()
{
    const int stabilityLevel = 1000;

	gl_Position = pvm * vec4(aPosition, 1.0);
	gl_Position.x = int(gl_Position.x * stabilityLevel) / float(stabilityLevel);
    gl_Position.y = int(gl_Position.y * stabilityLevel) / float(stabilityLevel);
    gl_Position.z = int(gl_Position.z * stabilityLevel) / float(stabilityLevel);

    textureCoordinates = aTextureCoordinates;
}